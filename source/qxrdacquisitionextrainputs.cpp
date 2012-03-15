#include "qxrdacquisitionextrainputs.h"
#include "qxrdacquisition.h"
#include "qxrdmutexlocker.h"
#include "qxrdnidaqplugininterface.h"
#include "qxrdimagedata.h"

QxrdAcquisitionExtraInputs::QxrdAcquisitionExtraInputs(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QxrdAcquisitionWPtr acq) :
  QObject(acq.data()),
  m_Enabled(QxrdSettingsSaverPtr(), this, "enabled", 0),
  m_ExtraInputs(saver, this, "extraInputs", 8),
  m_ChannelNames(saver, this, "channelNames", QStringList()),
  m_ChannelFlags(saver, this, "channelFlags", QcepIntList()),
  m_SampleRate(saver, this, "sampleRate", 1000.0),
  m_StartOffset(saver, this, "startOffset", QcepDoubleList()),
  m_EndOffset(saver, this, "endOffset", QcepDoubleList()),
  m_AcquireDelay(saver, this, "acquireDelay", 0.107),
  m_Experiment(doc),
  m_Acquisition(acq),
  m_NIDAQPlugin()
{
  QxrdAcquisitionPtr acqp(m_Acquisition);

  if (acqp) {
    m_NIDAQPlugin = acqp->nidaqPlugin();
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
}

void QxrdAcquisitionExtraInputs::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
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
