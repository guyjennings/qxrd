/******************************************************************
*
*  $Id: qxrdacquisitionperkinelmer.h,v 1.22 2010/04/09 22:29:38 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONPERKINELMER_H
#define QXRDACQUISITIONPERKINELMER_H

#include "qcepmacros.h"

#include "qxrdacquisitionsimulated.h"

#include <QMutex>
#include <QWaitCondition>
#include <QVector>

class QxrdAcquisitionThread;

class QxrdAcquisitionPerkinElmer : public QxrdAcquisitionSimulated
{
  Q_OBJECT;
public:
  QxrdAcquisitionPerkinElmer(QxrdDataProcessor *proc);
  ~QxrdAcquisitionPerkinElmer();

public slots:
  void acquire();
  void acquireDark();
  void cancel();
  void cancelDark();
  void initialize();
  void trigger();
//  int  status(double delay);

  void doRawSaveBenchmark();
  void doSubtractedSaveBenchmark();
  void doRawSubtractedSaveBenchmark();

signals:
  void acquireStarted(int dark);
  void acquireComplete(int dark);
  void oneReadoutModeChanged(int mode, double value);

public:
//  bool onEndFrame();
  void onEndAcquisition();
  QVector<double> readoutTimes();
  double  readoutTime() const;
  int acquisitionStatus(double time);
  int acquisitionCancel();
  void onEndFrameCallback();

  void returnImageToPool(QxrdInt16ImageData *img);
  void returnImageToPool(QxrdInt32ImageData *img);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);

protected:
  void acquisition(int isDark);
  void haltAcquire();

private slots:
  void onEndFrame();

private:
  void acquisitionError(int n);
  void acquisitionError(int ln, int n);
  void acquisitionInitError(int n);
  void acquisitionNSensorsError(int n);
  void allocateMemoryForAcquisition();
  void acquiredInt16Image();
  void acquiredInt32Image();

private:
  mutable QMutex         m_Mutex;
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;
//  int                    m_Cancelling;
//  int                    m_AcquireDark;
//  int                    m_NRows;
//  int                    m_NCols;
//  int                    m_ExposuresToSum;
//  int                    m_FilesInSequence;
  int                    m_CurrentExposure;
  int                    m_CurrentFile;
  int                    m_BufferSize;
  int                    m_BufferIndex;
  QVector<quint16>       m_Buffer;
//  QxrdDoubleImageData   *m_AcquiredData;
  QxrdInt16ImageData    *m_AcquiredInt16Data;
  QxrdInt32ImageData    *m_AcquiredInt32Data;
  QVector<double>        m_ReadoutTimes;
  QxrdInt16ImageQueue    m_FreeInt16Images;
  QxrdInt32ImageQueue    m_FreeInt32Images;
  QxrdInt16ImageQueue    m_PreTriggerInt16Images;
  QxrdInt32ImageQueue    m_PreTriggerInt32Images;

  int                    m_PROMID;
  int                    m_HeaderID;
  int                    m_CameraType;
  QString                m_CameraModel;
  int                    m_CurrentMode;
  int                    m_CurrentGain;

  HEADER_IDENT("$Id: qxrdacquisitionperkinelmer.h,v 1.22 2010/04/09 22:29:38 jennings Exp $");
};

#endif // QXRDACQUISITIONPERKINELMER_H

/******************************************************************
*
*  $Log: qxrdacquisitionperkinelmer.h,v $
*  Revision 1.22  2010/04/09 22:29:38  jennings
*  Removed file browser, added CMake support, build under VC
*
*  Revision 1.21  2009/09/28 22:06:09  jennings
*  Removed QxrdAcquisitionPerkinElmer::status
*
*  Revision 1.20  2009/09/21 18:49:24  jennings
*  Accelerate readout by only calling 'SetCameraMode' and 'SetCameraGain' if they change
*
*  Revision 1.19  2009/09/18 20:44:49  jennings
*  Add separate status functions for acquisition and processing, as well as an aggregated function
*  combining the status of the two.
*
*  Revision 1.18  2009/09/15 20:18:39  jennings
*  Added acquireCancel scripting command
*
*  Revision 1.17  2009/09/08 21:42:34  jennings
*  Added line numbers to acquisition error messages
*
*  Revision 1.16  2009/09/07 21:38:32  jennings
*  Moved acquiredInt{16,32]ImageAvailable signals into base class
*
*  Revision 1.15  2009/09/04 12:46:35  jennings
*  Added binning mode parameter
*  Added camera gain and binning mode user interfaces
*
*  Revision 1.14  2009/09/04 02:44:15  jennings
*  Implement pre-trigger acquisition
*
*  Revision 1.13  2009/09/03 21:16:24  jennings
*  Added properties and user interface elements for pre- and post- trigger counts
*  Added properties and user interface elements for fine-grained control of processing chain
*
*  Revision 1.12  2009/08/26 20:56:15  jennings
*  More Int16 and Int32 implementation
*
*  Revision 1.11  2009/08/26 16:58:53  jennings
*  Partial implementation of the separate Int16 and Int32 acquisition paths
*
*  Revision 1.10  2009/08/25 20:07:00  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.9  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.8  2009/07/14 20:07:00  jennings
*  Implemented simple simulated acquisition
*
*  Revision 1.7  2009/07/14 05:36:14  jennings
*  Moved data storage into individual channel objects, accelerated fitting (somewhat)
*
*  Revision 1.6  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.5  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

