#include "qxrdsingleacquisition.h"
#include "qxrddetector.h"
#include "qxrddebug.h"

QxrdSingleAcquisition::QxrdSingleAcquisition(QcepSettingsSaverWPtr saver,
                                             QxrdExperimentWPtr    doc,
                                             QxrdDataProcessorWPtr proc,
                                             QcepAllocatorWPtr     allocator)
  : QxrdAcquisition(saver, doc, proc, allocator),
    m_Detector()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSingleAcquisition::QxrdSingleAcquisition(%p)\n", this);
  }
}

QxrdSingleAcquisition::~QxrdSingleAcquisition()
{
#ifndef QT_NO_DEBUG
  printf("Deleting single acquisition\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSingleAcquisition::~QxrdSingleAcquisition(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdSingleAcquisition::~QxrdSingleAcquisition");
  }
}
void QxrdSingleAcquisition::setDetector(QxrdDetectorWPtr det)
{
  m_Detector = det;
}

QxrdDetectorPtr QxrdSingleAcquisition::detector()
{
  return m_Detector;
}

void QxrdSingleAcquisition::onExposureTimeChanged()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->onExposureTimeChanged();
  }
}

void QxrdSingleAcquisition::setupExposureMenu(QDoubleSpinBox *cb)
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->setupExposureMenu(cb, get_ExposureTime());
  }
}

void QxrdSingleAcquisition::beginAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->beginAcquisition();
  }
}

void QxrdSingleAcquisition::endAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->endAcquisition();
  }
}

void QxrdSingleAcquisition::shutdownAcquisition()
{
  QxrdDetectorPtr det(m_Detector);

  if (det) {
    det ->shutdownAcquisition();
  }
}

