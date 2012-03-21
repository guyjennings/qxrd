#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdimagedata.h"
#include "qxrdsettingssaver.h"
#include "qxrdacquisitionextrainputschannel.h"

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionWPtr acq) :
  QObject(acq.data()),
  m_Enabled(QxrdSettingsSaverPtr(), this, "enabled", 0, "Extra Inputs Enabled?"),
  m_ExtraInputs(saver, this, "extraInputs", 8, "Number of Extra Inputs"),
  m_ChannelNames(saver, this, "channelNames", QStringList(), "Extra Input Channel Names"),
  m_ChannelFlags(saver, this, "channelFlags", QcepIntList(), "Extra Input Channel Flags"),
  m_SampleRate(saver, this, "sampleRate", 1000.0, "Sampling Rate for Extra Inputs"),
  m_StartOffset(saver, this, "startOffset", QcepDoubleList(), "Start Offsets (in sec) for Extra Inputs"),
  m_EndOffset(saver, this, "endOffset", QcepDoubleList(), "End Offsets (in sec) for Extra Inputs"),
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

  m_Channels.clear();

  int n = settings->beginReadArray(section+"/channels");

  for (int i=0; i<n; i++) {
    appendChannel();

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
    m_NIDAQPlugin->prepareContinuousInput(get_SampleRate(),
                                  get_AcquireDelay(),
                                  acq->get_ExposureTime(),
                                  get_ChannelNames(),
                                  get_ChannelFlags(),
                                  get_StartOffset(),
                                  get_EndOffset());

    set_Enabled(true);
  }
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
  m_Channels.insert((ch < 0 ? m_Channels.size() : ch),
                    QxrdAcquisitionExtraInputsChannelPtr(
                      new QxrdAcquisitionExtraInputsChannel(m_Saver, m_Experiment, this)));
}

void QxrdAcquisitionExtraInputs::removeChannel(int ch)
{
  m_Channels.remove((ch < 0 ? m_Channels.size()-1 : ch));
}

void QxrdAcquisitionExtraInputs::acquire()
{
  if (get_Enabled()) {
    QxrdAcquisitionPtr acq(m_Acquisition);

    if (acq && m_NIDAQPlugin) {
      m_NIDAQPlugin->readContinuousInput(m_ChannelData);
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
  int n = get_ChannelNames().count();

  for (int i=0; i<n; i++) {
    res.append(evaluateChannel(i));
  }

  return res;
}

double QxrdAcquisitionExtraInputs::evaluateChannel(int ch)
{
  return sumChannel(ch);
}

QVector<double> QxrdAcquisitionExtraInputs::readChannel(int ch)
{
  return m_ChannelData.value(ch);
}

double QxrdAcquisitionExtraInputs::averageChannel(int ch)
{
  QVector<double> res = readChannel(ch);

  double n=res.count(), sum=0;

  for(int i=0; i<n; i++) {
    sum += res[i];
  }

  return sum/n;
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
