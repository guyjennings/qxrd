#include "qxrddebug.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qxrdacquisition.h"
#include "qcepmutexlocker.h"
#include "qxrdnidaqplugininterface.h"
#include "qcepimagedata.h"
#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionparameterpack.h"
#include <QTimer>

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QString name) :
  QcepSerializableObject(name),
  m_Enabled(this, "enabled", 0, "Extra Inputs Enabled?"),
  m_Skipping(this, "skipping", 0, "Skipping initial readout?"),
  m_SampleRate(this, "sampleRate", 1000.0, "Sampling Rate for Extra Inputs"),
  m_AcquireDelay(this, "acquireDelay", 0.107, "Delay between exposure end and Image available in QXRD"),
  m_ExposureTime(this, "exposureTime", 0.107, "Exposure time (in seconds)"),
  m_DeviceName(this, "deviceName", "", "NI-DAQ Device Name"),
  m_Channels(),
  m_NIDAQPlugin()
{
#ifndef QT_NO_DEBUG
  printf("Constructing acquisition extra inputs\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(%p)\n", this);
  }

  QxrdAcquisitionPtr acqp(acquisition());

  if (acqp) {
    m_NIDAQPlugin = acqp->nidaqPlugin();
  }

  connect(prop_SampleRate(), &QcepDoubleProperty::valueChanged,
          this, &QxrdAcquisitionExtraInputs::reinitiate);

  if (acqp) {
    connect(acqp->prop_ExposureTime(), &QcepDoubleProperty::valueChanged,
            this, &QxrdAcquisitionExtraInputs::reinitiate);
  }
}

QxrdAcquisitionExtraInputsPtr QxrdAcquisitionExtraInputs::newAcquisitionExtraInputs()
{
  QxrdAcquisitionExtraInputsPtr xtra(new QxrdAcquisitionExtraInputs("extraInputs"));

  return xtra;
}

void QxrdAcquisitionExtraInputs::addChildPtr(QcepObjectPtr child)
{
  QcepSerializableObject::addChildPtr(child);

  QxrdAcquisitionExtraInputsChannelPtr chan = qSharedPointerDynamicCast<QxrdAcquisitionExtraInputsChannel>(child);

  if (chan) {
    int ch = chan->get_ChannelNumber();

    int n = (ch < 0 ? m_Channels.size() : ch+1);

    m_Channels.insert(n, chan);
  }
}

void QxrdAcquisitionExtraInputs::removeChildPtr(QcepObjectPtr child)
{
  QxrdAcquisitionExtraInputsChannelPtr chan = qSharedPointerDynamicCast<QxrdAcquisitionExtraInputsChannel>(child);

  if (chan) {
    m_Channels.removeAll(chan);
  }

  QcepSerializableObject::removeChildPtr(child);
}

QxrdAcquisitionExtraInputs::~QxrdAcquisitionExtraInputs()
{
#ifndef QT_NO_DEBUG
  printf("Deleting acquisition extra inputs\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExtraInputs::~QxrdAcquisitionExtraInputs(%p)\n", this);
  }
}

QxrdAcquisitionWPtr QxrdAcquisitionExtraInputs::acquisition()
{
  QxrdAcquisitionPtr acq = qSharedPointerDynamicCast<QxrdAcquisition>(parentPtr());

  if (acq == NULL) {
    printMessage("QxrdAcquisitionExtraInputs::acquisition == NULL");
  }

  return acq;
}

void QxrdAcquisitionExtraInputs::setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}

QxrdNIDAQPluginInterfacePtr QxrdAcquisitionExtraInputs::nidaqPlugin() const
{
  return m_NIDAQPlugin;
}

void QxrdAcquisitionExtraInputs::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings);

  int n = settings->beginReadArray("channels");

  while (m_Channels.count() > n) {
    removeChannel();
  }

  while (m_Channels.count() < n) {
    appendChannel();
  }

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    QxrdAcquisitionExtraInputsChannelPtr chan = channel(i);

    if (chan) {
      chan->readSettings(settings);
    }
  }

  settings->endArray();

  initiate();
}

void QxrdAcquisitionExtraInputs::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings);

  settings->beginWriteArray("channels");

  int n = m_Channels.count();

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);
    QxrdAcquisitionExtraInputsChannelPtr chan = channel(i);

    if (chan) {
      chan->writeSettings(settings);
    }
  }

  settings->endArray();
}

void QxrdAcquisitionExtraInputs::prepareForAcquisition(QxrdAcquisitionParameterPackWPtr /*parms*/)
{
  if (!get_Enabled()) {
    initiate();
  }
}

void QxrdAcquisitionExtraInputs::initiate()
{
  QxrdAcquisitionPtr acq(acquisition());

  if (acq && m_NIDAQPlugin) {
    QStringList uniqueChannels;
    QVector<double> channelMinimum, channelMaximum;

    foreach (QxrdAcquisitionExtraInputsChannelPtr chanp, m_Channels) {
      if (chanp && chanp->get_Enabled()) {
        QString channame = chanp->get_ChannelName();
        double chanmin = chanp->get_Min();
        double chanmax = chanp->get_Max();

        int physChan = uniqueChannels.indexOf(channame);

        if (physChan < 0) {
          uniqueChannels.append(channame);
          physChan = uniqueChannels.count() - 1;
          channelMinimum.append(chanmin);
          channelMaximum.append(chanmax);
        } else {
          channelMinimum[physChan] = qMin(channelMinimum[physChan], chanmin);
          channelMaximum[physChan] = qMin(channelMaximum[physChan], chanmax);
        }

        chanp->set_PhysicalChannel(physChan);
      }
    }

    if (m_NIDAQPlugin->prepareContinuousInput(get_SampleRate(),
                                              get_AcquireDelay(),
                                              acq->get_ExposureTime(),
                                              uniqueChannels,
                                              channelMinimum,
                                              channelMaximum) == 0) {
      set_Skipping(true);
      set_ExposureTime(acq->get_ExposureTime());

      QTimer::singleShot(1000*(get_AcquireDelay() + acq->get_ExposureTime() + 1.0),
                         this, SLOT(timerDone()));

      set_Enabled(true);
    }
  }
}

void QxrdAcquisitionExtraInputs::reinitiate()
{
  if (get_Enabled()) {
    initiate();
  }
}

void QxrdAcquisitionExtraInputs::timerDone()
{
  set_Skipping(false);
}

QVector<QxrdAcquisitionExtraInputsChannelPtr> QxrdAcquisitionExtraInputs::channels() const
{
  return m_Channels;
}

QxrdAcquisitionExtraInputsChannelPtr QxrdAcquisitionExtraInputs::channel(int chan) const
{
  return m_Channels.value(chan);
}

void QxrdAcquisitionExtraInputs::appendChannel(int ch)
{
  QxrdAcquisitionExtraInputsChannelPtr chan =
      QxrdAcquisitionExtraInputsChannel::newAcquisitionExtraInputsChannel(ch);
  addChildPtr(chan);

  connect(chan.data(), &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded, this, &QxrdAcquisitionExtraInputs::reinitiate);

  QcepObjectPtr p(parentPtr());

  if (p) {
      p->propertyChanged(NULL);
  }

  emit channelCountChanged();
}

void QxrdAcquisitionExtraInputs::removeChannel(int ch)
{
  QxrdAcquisitionExtraInputsChannelPtr chan = channel(ch);

  if (chan) {
    removeChildPtr(chan);

    QcepObjectPtr p(parentPtr());

    if (p) {
      p->propertyChanged(NULL);
    }

    emit channelCountChanged();
  }
}

void QxrdAcquisitionExtraInputs::acquire()
{
  if (get_Enabled()) {
    if (!get_Skipping()) {
      QxrdAcquisitionPtr acq(acquisition());

      if (acq && m_NIDAQPlugin) {
        m_NIDAQPlugin->readContinuousInput();
      }

      foreach (QxrdAcquisitionExtraInputsChannelPtr chanp, m_Channels) {
        if (chanp && chanp->get_Enabled()) {
          chanp -> set_Value(
                chanp -> evaluateChannel());

          if (acq && chanp->get_Triggered()) {
            acq->set_Triggered(true);
          }
        }
      }

      emit newDataAvailable();
    }
  }
}

void QxrdAcquisitionExtraInputs::logToImage(QcepUInt16ImageDataPtr img)
{
  if (get_Enabled() && img) {
    img->set_ExtraInputs(evaluateChannels().toList());
  }
}

void QxrdAcquisitionExtraInputs::finish()
{
  QxrdAcquisitionPtr acq(acquisition());

  if (acq && m_NIDAQPlugin) {
    m_NIDAQPlugin->finishContinuousInput();
  }

  set_Enabled(false);
}

QVector<double> QxrdAcquisitionExtraInputs::evaluateChannels()
{
  QVector<double> res;

  for(int i=0; i<m_Channels.count(); i++) {
    QxrdAcquisitionExtraInputsChannelPtr chanp = channel(i);

    if (chanp && chanp->get_Enabled()) {
      double val = chanp->evaluateChannel();

      if (qcepDebug(DEBUG_EXTRAINPUTS)) {
        printMessage(tr("QxrdAcquisitionExtraInputs::evaluateChannels [%1] = %2").arg(i).arg(val));
      }

      res.append(val);
    }
  }

  return res;
}

double QxrdAcquisitionExtraInputs::evaluateChannel(int ch)
{
  QxrdAcquisitionExtraInputsChannelPtr chanp = channel(ch);

  if (chanp) {
    return chanp->evaluateChannel();
  } else {
    return 0;
  }
}

QVector<double> QxrdAcquisitionExtraInputs::readXChannel()
{
  QxrdAcquisitionExtraInputsChannelPtr chanp = channel(0);

  if (chanp) {
    QVector<double> res(chanp->readChannel().count());
    double offset = get_AcquireDelay();
    double rate   = get_SampleRate();

    for (int i=0; i<res.count(); i++) {
      res[i] = i/rate - offset;
    }

    return res;
  } else {
    return QVector<double>();
  }
}

QVector<double> QxrdAcquisitionExtraInputs::readChannel(int ch)
{
  QxrdAcquisitionExtraInputsChannelPtr chanp = channel(ch);

  if (chanp) {
    return chanp->readChannel();
  } else {
    return QVector<double>();
  }
}

double QxrdAcquisitionExtraInputs::averageChannel(int ch)
{
  QxrdAcquisitionExtraInputsChannelPtr chanp = channel(ch);

  if (chanp) {
    return chanp->averageChannel();
  } else {
    return 0;
  }
}

double QxrdAcquisitionExtraInputs::sumChannel(int ch)
{
  QVector<double> res = readChannel(ch);

  double n=res.count(), sum=0;

  for(int i=0; i<n; i++) {
    sum += res[i];
  }

  return sum;
}