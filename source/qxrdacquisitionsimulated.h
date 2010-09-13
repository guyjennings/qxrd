/******************************************************************
*
*  $Id: qxrdacquisitionsimulated.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONSIMULATED_H
#define QXRDACQUISITIONSIMULATED_H

#include "qcepmacros.h"

#include "qxrdacquisition.h"
#include <QTimer>

class QxrdAcquisitionSimulated : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionSimulated(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);

public slots:
  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

  void onTimerTimeout();

public:
  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);

signals:

private:
  void initialize();

  QTimer m_Timer;

  HEADER_IDENT("$Id: qxrdacquisitionsimulated.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDACQUISITIONSIMULATED_H

/******************************************************************
*
*  $Log: qxrdacquisitionsimulated.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.11  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.3.4.10  2010/05/19 20:37:31  jennings
*  ComboBox -> SpinBox
*
*  Revision 1.3.4.9  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3.4.8  2010/04/22 21:42:07  jennings
*  Simulated acquisition is beginning to work
*
*  Revision 1.3.4.7  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.3.4.6  2010/04/20 21:19:48  jennings
*  Added 'onExposureTimeChanged' etc. to simulated acquisition
*
*  Revision 1.3.4.5  2010/04/19 19:20:16  jennings
*  More reorganization of acquisition code
*
*  Revision 1.3.4.4  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.3.4.3  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.3.4.2  2010/04/15 19:34:24  jennings
*  Reorganization of acquisition object classes
*
*  Revision 1.3.4.1  2010/04/12 21:21:46  jennings
*  Version 0.4.0, some rearrangement of the Acquisition classes
*
*  Revision 1.3  2009/08/09 14:38:27  jennings
*  Fixed a couple of cvs Id keyword typos
*
*  Revision 1.2  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.1  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*
*******************************************************************/
