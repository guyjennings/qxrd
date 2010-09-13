/******************************************************************
*
*  $Id: qxrdacquisitionpilatus.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONPILATUS_H
#define QXRDACQUISITIONPILATUS_H

#include "qcepmacros.h"

#include "qxrdacquisition.h"

class QxrdAcquisitionThread;

class QxrdAcquisitionPilatus : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionPilatus(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);

private slots:
  void initialize();

  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

private:
  void beginAcquisition();
  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);

private:
  HEADER_IDENT("$Id: qxrdacquisitionpilatus.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDACQUISITIONPILATUS_H

/******************************************************************
*
*  $Log: qxrdacquisitionpilatus.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.5  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.1.2.4  2010/05/19 20:36:43  jennings
*  Changes needed after exposure dialog widget changed to spin box
*
*  Revision 1.1.2.3  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1.2.2  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.1.2.1  2010/04/21 19:48:41  jennings
*  Added QxrdAcquisitionAreaDetector, QxrdAcquisitionPilatus, QxrdDataProcessorCuda, QxrdDataProcessorThreaded
*  and QxrdPreferencesDialog - they don't do much yet, though.
*
*
*******************************************************************/

