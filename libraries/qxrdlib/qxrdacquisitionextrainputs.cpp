#include "qxrddebug.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qxrdacqcommon.h"
#include "qcepmutexlocker.h"
#include "qxrdnidaqplugininterface.h"
#include "qcepimagedata.h"
#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionparameterpack.h"
#include <QTimer>

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QString name) :
  inherited(name),
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
  connect(prop_SampleRate(), &QcepDoubleProperty::valueChanged,
          this, &QxrdAcquisitionExtraInputs::reinitiate);
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

void QxrdAcquisitionExtraInputs::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Acquisition = QxrdAcqCommon::findAcquisition(parent);

  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (acqp) {
    m_NIDAQPlugin = acqp->nidaqPlugin();
  }

  if (acqp) {
    connect(acqp->prop_ExposureTime(), &QcepDoubleProperty::valueChanged,
            this, &QxrdAcquisitionExtraInputs::reinitiate);
  }
}

void QxrdAcquisitionExtraInputs::setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}

QxrdNIDAQPluginInterface* QxrdAcquisitionExtraInputs::nidaqPlugin() const
{
  INIT_CHECK;

  return m_NIDAQPlugin;
}

void QxrdAcquisitionExtraInputs::readSettings(QSettings *settings)
{
  INIT_CHECK;

  inherited::readSettings(settings);

  int n = settings->beginReadArray("channels");

  //TODO: rewrite to use readObject...
//  while (m_Channels.count() > n) {
//    removeChannel();
//  }

//  while (m_Channels.count() < n) {
//    appendChannel();
//  }

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    QcepObjectPtr obj = QcepObject::readObject(sharedFromThis(), settings);

    if (obj) {
      QxrdAcquisitionExtraInputsChannelPtr chan =
          qSharedPointerDynamicCast<QxrdAcquisitionExtraInputsChannel>(obj);

      if (chan) {
        chan->setObjectName(tr("channel-%1").arg(i));

        appendChannel(chan);
      }
    }
  }

  settings->endArray();

  initiate();
}

void QxrdAcquisitionExtraInputs::writeSettings(QSettings *settings)
{
  INIT_CHECK;

  inherited::writeSettings(settings);

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
  INIT_CHECK;

  if (!get_Enabled()) {
    initiate();
  }
}

void QxrdAcquisitionExtraInputs::initiate()
{
  INIT_CHECK;

  QxrdAcqCommonPtr acq(m_Acquisition);

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
                         this, &QxrdAcquisitionExtraInputs::timerDone);

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
  INIT_CHECK;

  QxrdAcquisitionExtraInputsChannelPtr chan =
      QxrdAcquisitionExtraInputsChannel::newAcquisitionExtraInputsChannel(ch);

  if (chan) {
    int n = (ch < 0 ? m_Channels.size() : ch+1);

    m_Channels.insert(n, chan);

    connect(chan.data(), &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded, this, &QxrdAcquisitionExtraInputs::reinitiate);

    QcepObjectPtr p(parentPtr());

    if (p) {
      p->propertyChanged(NULL);
    }

    emit channelCountChanged();
  }
}

void QxrdAcquisitionExtraInputs::appendChannel(QxrdAcquisitionExtraInputsChannelWPtr chan)
{
  INIT_CHECK;

  if (chan) {
    m_Channels.append(chan);

    connect(chan.data(), &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded, this, &QxrdAcquisitionExtraInputs::reinitiate);

    QcepObjectPtr p(parentPtr());

    if (p) {
      p->propertyChanged(NULL);
    }

    emit channelCountChanged();
  }
}

void QxrdAcquisitionExtraInputs::removeChannel(int ch)
{
  INIT_CHECK;

  QxrdAcquisitionExtraInputsChannelPtr chan = channel(ch);

  if (chan) {
    m_Channels.removeAll(chan);

    QcepObjectPtr p(parentPtr());

    if (p) {
      p->propertyChanged(NULL);
    }

    emit channelCountChanged();
  }
}

void QxrdAcquisitionExtraInputs::acquire()
{
  INIT_CHECK;

  if (get_Enabled()) {
    if (!get_Skipping()) {
      QxrdAcqCommonPtr acq(m_Acquisition);

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
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && m_NIDAQPlugin) {
    m_NIDAQPlugin->finishContinuousInput();
  }

  set_Enabled(false);
}

QVector<double> QxrdAcquisitionExtraInputs::evaluateChannels()
{
  INIT_CHECK;

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
