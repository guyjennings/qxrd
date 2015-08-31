#include "qxrdmultipleacquisition.h"

QxrdMultipleAcquisition::QxrdMultipleAcquisition(QcepSettingsSaverWPtr saver,
                                                 QxrdExperimentWPtr    doc,
                                                 QxrdDataProcessorWPtr proc,
                                                 QcepAllocatorWPtr     allocator)
      : QxrdAcquisition(saver, doc, proc, allocator)
{

}

