#include "qxrdsynchronizedacquisition.h"
#include "qcepmutexlocker.h"
#include "qxrdacqcommon.h"
#include "qwt_math.h"
#include "qxrdacqcommon.h"
#include "qxrdacquisitionparameterpack.h"
#include "qxrdnidaq.h"
#include "qxrdsynchronizeddetectorchannel.h"
#include "qxrdsynchronizedoutputchannel.h"
#include "qxrdsynchronizedinputchannel.h"

QxrdSynchronizedAcquisition::QxrdSynchronizedAcquisition(QString name) :
  QcepObject(name),
  m_DetectorCount(this, "detectorCount", 0, "Number of detector synchronization channels"),
  m_OutputCount(this, "outputCount", 0, "Number of synchronized output channels"),
  m_InputCount(this, "inputCount", 0, "Number of synchronized input channels"),
  m_PrimaryCounter(this, "primaryCounter", -1, "Primary Counter Device Number"),
  m_OutputSampleRate(this, "outputSampleRate", 1000, "Sample Rate for outputs (Hz)"),
  m_InputSampleRate(this, "inputSampleRate", 1000, "Sample Rate for inputs (Hz)")
{
#ifndef QT_NO_DEBUG
  printf("Constructing synchronized acquisition\n");
#endif
}

void QxrdSynchronizedAcquisition::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Acquisition = QxrdAcqCommon::findAcquisition(parent);
}

QxrdSynchronizedAcquisition::~QxrdSynchronizedAcquisition()
{
#ifndef QT_NO_DEBUG
  printf("Deleting synchronized acquisition\n");
#endif
}

void QxrdSynchronizedAcquisition::setupAcquisition()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    connect(acq->prop_ExposureTime(),  &QcepDoubleProperty::valueChanged,
            this,                      &QxrdSynchronizedAcquisition::updateWaveforms);

    connect(acq->prop_PhasesInGroup(), &QcepIntProperty::valueChanged,
            this,                      &QxrdSynchronizedAcquisition::updateWaveforms);

    connect(this,                      &QcepObject::propertyWasChanged,
            this,                      &QxrdSynchronizedAcquisition::updateWaveforms);

    updateWaveforms();
  }
}

void QxrdSynchronizedAcquisition::registerMetaTypes()
{
  qRegisterMetaType<QxrdSynchronizedAcquisition*>("QxrdSynchronizedAcquisition*");
  qRegisterMetaType<QxrdSynchronizedDetectorChannel*>("QxrdSynchronizedDetectorChannel*");
  qRegisterMetaType<QxrdSynchronizedOutputChannel*>("QxrdSynchronizedOutputChannel*");
  qRegisterMetaType<QxrdSynchronizedInputChannel*>("QxrdSynchronizedInputChannel*");
}

QxrdAcqCommonWPtr QxrdSynchronizedAcquisition::acquisition()
{
  return m_Acquisition;
}

void QxrdSynchronizedAcquisition::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  int nDet = settings -> beginReadArray("detectors");

  for (int i=0; i<nDet; i++) {
    settings -> setArrayIndex(i);

    QxrdSynchronizedDetectorChannelPtr det(
          new QxrdSynchronizedDetectorChannel(tr("detector-%1").arg(i)));

    if (det) {
      det -> initialize(sharedFromThis());

      det -> readSettings(settings);

      m_Detectors.append(det);
    }
  }

  settings -> endArray();

  int nOut = settings -> beginReadArray("outputs");

  for (int i=0; i<nOut; i++) {
    settings -> setArrayIndex(i);

    QxrdSynchronizedOutputChannelPtr out(
          new QxrdSynchronizedOutputChannel(tr("output-%1").arg(i)));

    if (out) {
      out -> initialize(sharedFromThis());

      out -> readSettings(settings);

      m_Outputs.append(out);
    }
  }

  settings -> endArray();

  int nInp = settings -> beginReadArray("inputs");

  for (int i=0; i<nInp; i++) {
    settings -> setArrayIndex(i);

    QxrdSynchronizedInputChannelPtr inp(
          new QxrdSynchronizedInputChannel(tr("input-%1").arg(i)));

    if (inp) {
      inp -> initialize(sharedFromThis());

      inp -> readSettings(settings);

      m_Inputs.append(inp);
    }
  }

  settings -> endArray();
}

void QxrdSynchronizedAcquisition::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  settings->beginWriteArray("detectors");

  int nDets = m_Detectors.count();

  for (int i=0; i<nDets; i++) {
    settings->setArrayIndex(i);
    QxrdSynchronizedDetectorChannelPtr det = m_Detectors.value(i);

    if (det) {
      det->writeSettings(settings);
    }
  }

  settings->endArray();

  settings->beginWriteArray("outputs");

  int nOut = m_Outputs.count();

  for (int i=0; i<nOut; i++) {
    settings->setArrayIndex(i);
    QxrdSynchronizedOutputChannelPtr out = m_Outputs.value(i);

    if (out) {
      out->writeSettings(settings);
    }
  }

  settings->endArray();

  settings->beginWriteArray("inputs");

  int nInp = m_Inputs.count();

  for (int i=0; i<nInp; i++) {
    settings->setArrayIndex(i);
    QxrdSynchronizedInputChannelPtr inp = m_Inputs.value(i);

    if (inp) {
      inp->writeSettings(settings);
    }
  }

  settings->endArray();
}

QString QxrdSynchronizedAcquisition::primaryCounterName()
{
  int n = get_PrimaryCounter();

  QString res = QString();

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    res = nidaq->detectorDeviceName(n);
  }

  return res;
}

int QxrdSynchronizedAcquisition::detectorCount()
{
  return m_Detectors.count();
}

QxrdSynchronizedDetectorChannelPtr QxrdSynchronizedAcquisition::detector(int n)
{
  return m_Detectors.value(n);
}

int QxrdSynchronizedAcquisition::detectorDeviceCount()
{
  int res = 0;

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    res = nidaq->detectorDeviceCount();
  }

  return res;
}

QString QxrdSynchronizedAcquisition::detectorDeviceName(int n)
{
  QString res = QString();

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    res = nidaq->detectorDeviceName(n);
  }

  return res;
}

void QxrdSynchronizedAcquisition::newDetector(int before)
{
  QxrdSynchronizedDetectorChannelPtr det(
        new QxrdSynchronizedDetectorChannel(tr("detector")));

  if (det) {
    det->initialize(sharedFromThis());

    m_Detectors.insert(before, det);
  }

  renumberDetectors();
}

void QxrdSynchronizedAcquisition::deleteDetector(int n)
{
  m_Detectors.removeAt(n);

  renumberDetectors();
}

void QxrdSynchronizedAcquisition::renumberDetectors()
{
  for (int i=0; i<detectorCount(); i++) {
    QxrdSynchronizedDetectorChannelPtr det(detector(i));

    if (det) {
      det -> setObjectName(tr("detector-%1").arg(i));
    }
  }

  set_DetectorCount(detectorCount());

  emit detectorCountChanged(detectorCount());
}

int QxrdSynchronizedAcquisition::outputCount()
{
  return m_Outputs.count();
}

QxrdSynchronizedOutputChannelPtr QxrdSynchronizedAcquisition::output(int n)
{
  return m_Outputs.value(n);
}

int QxrdSynchronizedAcquisition::outputDeviceCount()
{
  int res = 0;

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    res = nidaq->outputDeviceCount();
  }

  return res;
}

QString QxrdSynchronizedAcquisition::outputDeviceName(int n)
{
  QString res = QString();

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    res = nidaq->outputDeviceName(n);
  }

  return res;
}

void QxrdSynchronizedAcquisition::newOutput(int before)
{
  QxrdSynchronizedOutputChannelPtr out(
        new QxrdSynchronizedOutputChannel(tr("output")));

  if (out) {
    out -> initialize(sharedFromThis());

    m_Outputs.insert(before, out);
  }

  renumberOutputs();
}

void QxrdSynchronizedAcquisition::deleteOutput(int n)
{
  m_Outputs.removeAt(n);

  renumberOutputs();
}

void QxrdSynchronizedAcquisition::renumberOutputs()
{
  for (int i=0; i<outputCount(); i++) {
    QxrdSynchronizedOutputChannelPtr out(output(i));

    if (out) {
      out -> setObjectName(tr("output-%1").arg(i));
    }
  }

  set_OutputCount(outputCount());

  emit outputCountChanged(outputCount());
}

int QxrdSynchronizedAcquisition::inputCount()
{
  return m_Inputs.count();
}

QxrdSynchronizedInputChannelPtr QxrdSynchronizedAcquisition::input(int n)
{
  return m_Inputs.value(n);
}

int QxrdSynchronizedAcquisition::inputDeviceCount()
{
  int res = 0;

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    res = nidaq->inputDeviceCount();
  }

  return res;
}

QString QxrdSynchronizedAcquisition::inputDeviceName(int n)
{
  QString res = QString();

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    res = nidaq->inputDeviceName(n);
  }

  return res;
}

void QxrdSynchronizedAcquisition::newInput(int before)
{
  QxrdSynchronizedInputChannelPtr inp(
        new QxrdSynchronizedInputChannel(tr("input")));

  if (inp) {
    inp -> initialize(sharedFromThis());

    m_Inputs.insert(before, inp);
  }

  renumberInputs();
}

void QxrdSynchronizedAcquisition::deleteInput(int n)
{
  m_Inputs.removeAt(n);

  renumberInputs();
}

void QxrdSynchronizedAcquisition::renumberInputs()
{
  for (int i=0; i<inputCount(); i++) {
    QxrdSynchronizedInputChannelPtr inp(input(i));

    if (inp) {
      inp -> setObjectName(tr("input-%1").arg(i));
    }
  }

  set_InputCount(inputCount());

  emit inputCountChanged(inputCount());
}

QVector<double> QxrdSynchronizedAcquisition::evaluateInputs()
{
  QVector<double> res;

  for (int i=0; i<inputCount(); i++) {
    QxrdSynchronizedInputChannelPtr inp(input(i));

    if (inp) {
      res.append(inp -> evaluateInput());
    }
  }

  return res;
}

double QxrdSynchronizedAcquisition::evaluateInput(int ch)
{
  double res = 0;

  QxrdSynchronizedInputChannelPtr inp(input(ch));

  if (inp) {
    res = inp -> evaluateInput();
  }

  return res;
}

void QxrdSynchronizedAcquisition::setNIDAQPlugin(QxrdNIDAQWPtr nidaqPlugin)
{
  m_NIDAQPlugin = nidaqPlugin;
}

QxrdNIDAQWPtr QxrdSynchronizedAcquisition::nidaqPlugin() const
{
  return m_NIDAQPlugin;
}

QxrdAcquisitionParameterPackWPtr QxrdSynchronizedAcquisition::parms()
{
  return m_AcquisitionParms;
}

void QxrdSynchronizedAcquisition::finishedAcquisition()
{
  m_AcquisitionParms = QxrdAcquisitionParameterPackWPtr();
//  m_SyncMode = 0;
}

void QxrdSynchronizedAcquisition::updateWaveforms()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdAcquisitionParameterPackPtr parms(acq->acquisitionParameterPack());

    for (int i=0; i<outputCount(); i++) {
      QxrdSynchronizedOutputChannelPtr out(output(i));

      if (out) {
        out->recalculateWaveform(
              qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
      }
    }

    for (int i=0; i<inputCount(); i++) {
      QxrdSynchronizedInputChannelPtr inp(input(i));

      if (inp) {
        inp->prepareForInput(
              qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
      }
    }

    QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

    if (nidaq) {
      nidaq->updateSyncWaveforms(
            qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
    }
  }

  emit waveformsChanged();
}

void QxrdSynchronizedAcquisition::prepareForDarkAcquisition(QxrdDarkAcquisitionParameterPackWPtr parms)
{
//  m_SyncMode = 0;
  for (int i=0; i<outputCount(); i++) {
    QxrdSynchronizedOutputChannelPtr out(output(i));

    if (out) {
      out->disableWaveform();
    }
  }

  for (int i=0; i<inputCount(); i++) {
    QxrdSynchronizedInputChannelPtr inp(input(i));

    if (inp) {
      inp->prepareForDarkInput(
            qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
    }
  }

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    nidaq->prepareForDarkAcquistion(
          qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
  }

  emit waveformsChanged();
}

void QxrdSynchronizedAcquisition::prepareForAcquisition(QxrdAcquisitionParameterPackWPtr parms)
{
  for (int i=0; i<outputCount(); i++) {
    QxrdSynchronizedOutputChannelPtr out(output(i));

    if (out) {
      out->recalculateWaveform(
            qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
    }
  }

  for (int i=0; i<inputCount(); i++) {
    QxrdSynchronizedInputChannelPtr inp(input(i));

    if (inp) {
      inp->prepareForInput(
            qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
    }
  }

  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

  if (nidaq) {
    nidaq -> prepareForAcquisition(
          qSharedPointerDynamicCast<QxrdSynchronizedAcquisition>(sharedFromThis()), parms);
  }

  emit waveformsChanged();
}

//void QxrdSynchronizedAcquisition::acquiredFrameAvailable(int frameNumber)
//{
//  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

////  if (nidaq) {
////    nidaq->pulseOutput();
////  }

//  QxrdAcqCommonPtr acq(m_Acquisition);
//  QxrdAcquisitionParameterPackPtr parms(m_AcquisitionParms);

//  if (acq && parms) {
//    if (acq->acquisitionStatus(0.0) == 0) {
////      printf("QxrdSynchronizedAcquisition::acquiredFrameAvailable(%d)\n", frameNumber);

//      int skipBefore = parms->skipBefore();
//      int skipBetween = parms->skipBetween();
//      int nPhases = parms->nphases();
//      int nSummed = parms->nsummed();
//      int nGroups = parms->postTrigger();
//      int perGroup = nPhases*nSummed+skipBetween;
//      int inGroup = (frameNumber-skipBefore) % perGroup;
//      int phase = inGroup % nPhases;

//      if (nPhases > 0) {
//        if ((frameNumber >= skipBefore) && (frameNumber < (nGroups*perGroup-skipBetween+skipBefore))) {
//          if (inGroup < nPhases*nSummed) {
//            if (phase == 0) {
//              if (nidaq) {
////                printf("Triggered on frame %d\n", frameNumber);
////                nidaq->triggerAnalogWaveform();
//              }
//            }
//          }
//        }
//      }
////      printf("elapsed[%d] %d msec\n", currentPhase, tick.restart());
//    }
//  }
//}

//void QxrdSynchronizedAcquisition::setManualOutput()
//{
//  QxrdAcqCommonPtr acq(m_Acquisition);
//  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);

//  if (acq && nidaq) {
//    QString fullChannel = get_SyncAcquisitionOutputChannel();

//    acq->printMessage(tr("Manually Setting %1 to %2 V")
//                                .arg(fullChannel)
//                                .arg(get_SyncAcquisitionManualValue()));

//    nidaq->setAnalogOutput(fullChannel, get_SyncAcquisitionManualValue());
//  }
//}

//void QxrdSynchronizedAcquisition::triggerOnce()
//{
//  QxrdAcqCommonPtr acq(m_Acquisition);
//  QxrdNIDAQPtr nidaq(m_NIDAQPlugin);
//  QxrdAcquisitionParameterPackPtr parms(m_AcquisitionParms);

//  if (acq && nidaq && parms) {
//    prepareForAcquisition(parms);
//    nidaq->triggerAnalogWaveform();
//  }
//}
