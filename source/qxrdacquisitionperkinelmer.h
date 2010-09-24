/******************************************************************
*
*  $Id: qxrdacquisitionperkinelmer.h,v 1.4 2010/09/24 22:29:37 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONPERKINELMER_H
#define QXRDACQUISITIONPERKINELMER_H

//#ifdef HAVE_PERKIN_ELMER

#include "qcepmacros.h"

#include "qxrdacquisition.h"
#include "qxrdperkinelmerplugininterface.h"

#include <QMutex>
#include <QVector>

class QxrdAcquisitionThread;

class QxrdAcquisitionPerkinElmer : public QxrdAcquisition
{
  Q_OBJECT;
public:
  QxrdAcquisitionPerkinElmer(QxrdDataProcessorPtr proc, QxrdAllocatorPtr allocator);
//  ~QxrdAcquisitionPerkinElmer();

private slots:
  void onExposureTimeChanged(double newTime);
  void onBinningModeChanged(int newMode);
  void onCameraGainChanged(int newGain);

private:
  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);
  bool checkPluginAvailable();

public slots:
  void initialize();
  void onEndFrame(int counter, unsigned int n1, unsigned int n2);
  void onEndFrameCallback();

private:
  void acquisitionInitError(int n);
  void acquisitionNSensorsError(int n);

private:
  mutable QMutex         m_Mutex;
  int                    m_BufferSize;
  int                    m_BufferIndex;
  QVector<quint16>       m_Buffer;
  QVector<double>        m_ReadoutTimes;

  int                    m_PROMID;
  int                    m_HeaderID;
  int                    m_CameraType;
  QString                m_CameraModel;
  int                    m_CurrentGain;

  QAtomicInt             m_Counter;

  QxrdPerkinElmerPluginInterface *m_PerkinElmer;

  HEADER_IDENT("$Id: qxrdacquisitionperkinelmer.h,v 1.4 2010/09/24 22:29:37 jennings Exp $");
};

//#endif // HAVE_PERKIN_ELMER

#endif // QXRDACQUISITIONPERKINELMER_H

/******************************************************************
*
*  $Log: qxrdacquisitionperkinelmer.h,v $
*  Revision 1.4  2010/09/24 22:29:37  jennings
*  Work on NSIS installer
*  Fixed startup problem on debug builds when calling QxrdAcquisitionThread->initialize()
*
*  Revision 1.3  2010/09/23 19:57:32  jennings
*  Modified plugins for perkin elmer - now works in 64 bit mode
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.22.2.16  2010/05/27 21:13:14  jennings
*  Added more debug flags
*  Tried to detect when processing is too slow for acquisition
*
*  Revision 1.22.2.15  2010/05/26 19:20:43  jennings
*  Modified interface to XISL.dll so that it is accessed via 'LoadLibrary' and
*  'GetProcAddress' rather than being explicitly linked.  Should allow
*  qxrd to run if the PE libraries are not present.
*
*  Revision 1.22.2.14  2010/05/26 15:41:38  jennings
*  Adding support for weak linking of XISL dll
*
*  Revision 1.22.2.13  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.22.2.12  2010/05/19 21:15:34  jennings
*  PE acquisition debugging
*
*  Revision 1.22.2.11  2010/05/19 01:12:14  jennings
*  Tidied up PE acquisition code
*
*  Revision 1.22.2.10  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.22.2.9  2010/04/24 19:38:40  jennings
*  Added HAVE_PERKIN_ELMER config symbol to conditionalise support for
*  Perkin Elmer detector.
*
*  Revision 1.22.2.8  2010/04/22 19:40:33  jennings
*  Further rearrangement of acquisition code
*
*  Revision 1.22.2.7  2010/04/20 21:19:29  jennings
*  Added 'onExposureTimeChanged' etc. to PE acquisition
*
*  Revision 1.22.2.6  2010/04/19 21:48:26  jennings
*  More rearrangement
*
*  Revision 1.22.2.5  2010/04/19 19:20:16  jennings
*  More reorganization of acquisition code
*
*  Revision 1.22.2.4  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.22.2.3  2010/04/17 05:13:58  jennings
*  Partially working
*
*  Revision 1.22.2.2  2010/04/15 19:34:24  jennings
*  Reorganization of acquisition object classes
*
*  Revision 1.22.2.1  2010/04/12 21:21:46  jennings
*  Version 0.4.0, some rearrangement of the Acquisition classes
*
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

