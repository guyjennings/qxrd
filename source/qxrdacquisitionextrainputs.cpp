#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdimagedata.h"
#include "qxrdsettingssaver.h"
#include "qxrdacquisitionextrainputschannel.h"
#include <QTimer>

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionWPtr acq) :
  QObject(acq.data()),
  m_Enabled(QxrdSettingsSaverPtr(), this, "enabled", 0, "Extra Inputs Enabled?"),
  m_Skipping(QxrdSettingsSaverPtr(), this, "skipping", 0, "Skipping initial readout?"),
  m_SampleRate(saver, this, "sampleRate", 1000.0, "Sampling Rate for Extra Inputs"),
  m_AcquireDelay(saver, this, "acquireDelay", 0.107, "Delay between exposure end and Image available in QXRD"),
  m_Experiment(doc),
  m_Acquisition(acq),
  m_Saver(saver),
  m_Channels(),
  m_NIDAQPlugin()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_NIDAQPlugin = acqp->nidaqPlugin();
  }

  setObjectName("extraInputs");

  connect(prop_SampleRate(), SIGNAL(valueChanged(double,int)), this, SLOT(reinitialize()));

  if (acqp) {
    connect(acqp->prop_ExposureTime(), SIGNAL(valueChanged(double,int)), this, SLOT(reinitialize()));
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
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  int n = settings->beginReadArray(section+"/channels");

  while (m_Channels.count() > n) {
    removeChannel();
  }

  while (m_Channels.count() < n) {
    appendChannel();
  }

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    m_Channels[i]->readSettings(settings, "");
  }

  settings->endArray();
}

void QxrdAcquisitionExtraInputs::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  settings->beginWriteArray(section+"/channels");

  int n = m_Channels.count();

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);
    m_Channels[i]->writeSettings(settings, "");
  }

  settings->endArray();
}

void QxrdAcquisitionExtraInputs::initialize()
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq && m_NIDAQPlugin) {
    QStringList uniqueChannels;
    QVector<double> channelMinimum, channelMaximum;

    foreach (QxrdAcquisitionExtraInputsChannelPtr chanp, m_Channels) {
      if (chanp->get_Enabled()) {
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

      QTimer::singleShot(1000*(get_AcquireDelay() + acq->get_ExposureTime() + 1.0),
                         this, SLOT(timerDone()));

      set_Enabled(true);
    }
  }
}

void QxrdAcquisitionExtraInputs::reinitialize()
{
  if (get_Enabled()) {
    initialize();
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
  int n = (ch < 0 ? m_Channels.size() : ch);

  m_Channels.insert(n,
                    QxrdAcquisitionExtraInputsChannelPtr(
                      chan = new QxrdAcquisitionExtraInputsChannel(n, m_Saver, m_Experiment, this)));

  connect(chan, SIGNAL(reinitializeNeeded()), this, SLOT(reinitialize()));
}

void QxrdAcquisitionExtraInputs::removeChannel(int ch)
{
  m_Channels.remove((ch < 0 ? m_Channels.size()-1 : ch));
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
        if (chanp->get_Enabled()) {
          chanp -> set_Value(
                chanp -> evaluateChannel());
        }
      }

      emit newDataAvailable();
    }
  }
}

void QxrdAcquisitionExtraInputs::logToImage(QxrdInt16ImageDataPtr img)
{
  if (get_Enabled() && img) {
    img->set_Normalization(evaluateChannels());
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

  foreach (QxrdAcquisitionExtraInputsChannelPtr chanp, m_Channels) {
    if (chanp->get_Enabled()) {
      res.append(chanp->evaluateChannel());
    }
  }

  return res;
}

double QxrdAcquisitionExtraInputs::evaluateChannel(int ch)
{
  QxrdAcquisitionExtraInputsChannelPtr chanp = m_Channels.value(ch);

  if (chanp) {
    return chanp->evaluateChannel();
  } else {
    return 0;
  }
}

QVector<double> QxrdAcquisitionExtraInputs::readXChannel()
{
  QxrdAcquisitionExtraInputsChannelPtr chanp = m_Channels.value(0);

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
  QxrdAcquisitionExtraInputsChannelPtr chanp = m_Channels.value(ch);

  if (chanp) {
    return chanp->readChannel();
  } else {
    return QVector<double>();
  }
}

double QxrdAcquisitionExtraInputs::averageChannel(int ch)
{
  QxrdAcquisitionExtraInputsChannelPtr chanp = m_Channels.value(ch);

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