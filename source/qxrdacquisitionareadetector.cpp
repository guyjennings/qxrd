/******************************************************************
*
*  $Id: qxrdacquisitionareadetector.cpp,v 1.2 2010/09/13 20:00:38 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionareadetector.h"

QxrdAcquisitionAreaDetector::QxrdAcquisitionAreaDetector(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator)
  : QxrdAcquisition(proc, allocator),
    SOURCE_IDENT("$Id: qxrdacquisitionareadetector.cpp,v 1.2 2010/09/13 20:00:38 jennings Exp $")
{
}

void QxrdAcquisitionAreaDetector::initialize()
{
  QxrdAcquisition::initialize();
}

void QxrdAcquisitionAreaDetector::beginAcquisition()
{
}

void QxrdAcquisitionAreaDetector::onExposureTimeChanged(double /*newTime*/)
{
}

void QxrdAcquisitionAreaDetector::onBinningModeChanged(int /*newMode*/)
{
}

void QxrdAcquisitionAreaDetector::onCameraGainChanged(int /*newGain*/)
{
}

void QxrdAcquisitionAreaDetector::setupExposureMenu(QDoubleSpinBox * /*cb*/)
{
}

void QxrdAcquisitionAreaDetector::setupCameraGainMenu(QComboBox * /*cb*/)
{
}

void QxrdAcquisitionAreaDetector::setupCameraBinningModeMenu(QComboBox * /*cb*/)
{
}

/******************************************************************
*
*  $Log: qxrdacquisitionareadetector.cpp,v $
*  Revision 1.2  2010/09/13 20:00:38  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:36  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.9  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.8  2010/05/21 20:04:41  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.1.2.7  2010/05/21 18:51:22  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.6  2010/05/19 20:36:43  jennings
*  Changes needed after exposure dialog widget changed to spin box
*
*  Revision 1.1.2.5  2010/04/27 15:19:42  jennings
*  More changes to help using QSharedPointer
*
*  Revision 1.1.2.4  2010/04/26 00:37:09  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.3  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.1.2.2  2010/04/21 20:51:21  jennings
*  Added detectorType and detectorTypeName properties to each detector type
*
*  Revision 1.1.2.1  2010/04/21 19:48:41  jennings
*  Added QxrdAcquisitionAreaDetector, QxrdAcquisitionPilatus, QxrdDataProcessorCuda, QxrdDataProcessorThreaded
*  and QxrdPreferencesDialog - they don't do much yet, though.
*
*
*******************************************************************/

