#include "qxrdmultipleacquisition.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorproxy.h"
#include "qxrddebug.h"
#include "qcepmutexlocker.h"

QxrdMultipleAcquisition::QxrdMultipleAcquisition(QcepSettingsSaverWPtr saver,
                                                 QxrdExperimentWPtr    doc,
                                                 QxrdDataProcessorWPtr proc,
                                                 QcepAllocatorWPtr     allocator)
      : QxrdAcquisition(saver, doc, proc, allocator)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdMultipleAcquisition::QxrdMultipleAcquisition(%p)\n", this);
  }
}

QxrdMultipleAcquisition::~QxrdMultipleAcquisition()
{
#ifndef QT_NO_DEBUG
  printf("Deleting multiple acquisition\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdMultipleAcquisition::~QxrdMultipleAcquisition(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdMultipleAcquisition::~QxrdMultipleAcquisition");
  }
}

//void QxrdMultipleAcquisition::acquire()
//{
//  foreach (QxrdDetectorPtr det, m_Detectors) {
//    if (det && det->get_Enabled()) {
//      det->acquire();
//    }
//  }

//  prop_FileIndex()->incValue(get_PostTriggerFiles());
//}

