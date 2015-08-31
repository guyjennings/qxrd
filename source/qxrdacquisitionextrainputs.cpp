#include "qxrddebug.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qxrdacquisition.h"
#include "qcepmutexlocker.h"
#include "qxrdnidaqplugininterface.h"
#include "qcepimagedata.h"
#include "qcepsettingssaver.h"
#include "qxrdacquisitionextrainputschannel.h"
#include "qxrdacquisitionparameterpack.h"
#include <QTimer>

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionWPtr acq) :
  QcepObject("extraInputs", NULL),
  m_Enabled(QcepSettingsSaverPtr(), this, "enabled", 0, "Extra Inputs Enabled?"),
  m_Skipping(QcepSettingsSaverPtr(), this, "skipping", 0, "Skipping initial readout?"),
  m_SampleRate(saver, this, "sampleRate", 1000.0, "Sampling Rate for Extra Inputs"),
  m_AcquireDelay(saver, this, "acquireDelay", 0.107, "Delay between exposure end and Image available in QXRD"),
  m_ExposureTime(QcepSettingsSaverPtr(), this, "exposureTime", 0.107, "Exposure time (in seconds)"),
  m_DeviceName(saver, this, "deviceName", "", "NI-DAQ Device Name"),
  m_Experiment(doc),
  m_Acquisition(acq),
  m_Saver(saver),
  m_Channels(),
  m_NIDAQPlugin()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(%p)\n", this);
  }

  QxrdAcquisitionPtr acqp(m_Acquisition);

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

void QxrdAcquisitionExtraInputs::initialize(QxrdAcquisitionExtraInputsWPtr extra)
{
  m_ExtraInputs = extra;
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

void QxrdAcquisitionExtraInputs::setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}

QxrdNIDAQPluginInterfacePtr QxrdAcquisitionExtraInputs::nidaqPlugin() const
{
  return m_NIDAQPlugin;
}

void QxrdAcquisitionExtraInputs::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  int n = settings->beginReadArray(section+"/channels");

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
      chan->readSettings(settings, "");
    }
  }

  settings->endArray();

  initiate();
}

void QxrdAcquisitionExtraInputs::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  settings->beginWriteArray(section+"/channels");

  int n = m_Channels.count();

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);
    QxrdAcquisitionExtraInputsChannelPtr chan = channel(i);

    if (chan) {
      chan->writeSettings(settings, "");
    }
  }

  settings->endArray();
}

void QxrdAcquisitionExtraInputs::printMessage(QString msg, QDateTime ts) const
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->printMessage(msg, ts);
  }
}

void QxrdAcquisitionExtraInputs::criticalMessage(QString msg, QDateTime ts) const
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->criticalMessage(msg, ts);
  }
}

void QxrdAcquisitionExtraInputs::statusMessage(QString msg, QDateTime ts) const
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    acq->statusMessage(msg, ts);
  }
}

void QxrdAcquisitionExtraInputs::prepareForAcquisition(QxrdAcquisitionParameterPackWPtr /*parms*/)
{
  if (!get_Enabled()) {
    initiate();
  }
}

void QxrdAcquisitionExtraInputs::initiate()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

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
  QxrdAcquisitionExtraInputsChannel *chan = 0;
  int n = (ch < 0 ? m_Channels.size() : ch+1);

  m_Channels.insert(n,
                    QxrdAcquisitionExtraInputsChannelPtr(
                        chan = new QxrdAcquisitionExtraInputsChannel(n, m_Saver, m_Experiment, m_ExtraInputs)));

  connect(chan, &QxrdAcquisitionExtraInputsChannel::reinitiateNeeded, this, &QxrdAcquisitionExtraInputs::reinitiate);

  QcepSettingsSaverPtr saver(m_Saver);

  if (saver) {
      saver->changed();
  }
}

void QxrdAcquisitionExtraInputs::removeChannel(int ch)
{
  m_Channels.remove((ch < 0 ? m_Channels.size()-1 : ch));

  QcepSettingsSaverPtr saver(m_Saver);

  if (saver) {
      saver->changed();
  }
}

void QxrdAcquisitionExtraInputs::acquire()
{
  if (get_Enabled()) {
    if (!get_Skipping()) {
      QxrdAcquisitionPtr acq(m_Acquisition);

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

void QxrdAcquisitionExtraInputs::logToImage(QcepInt16ImageDataPtr img)
{
  if (get_Enabled() && img) {
    img->set_ExtraInputs(evaluateChannels());
  }
}

void QxrdAcquisitionExtraInputs::finish()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq && m_NIDAQPlugin) {
    m_NIDAQPlugin->finishContinuousInput();
  }

  set_Enabled(false);
}

QcepDoubleList QxrdAcquisitionExtraInputs::evaluateChannels()
{
  QcepDoubleList res;

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
