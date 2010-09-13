/******************************************************************
*
*  $Id: qxrdwindow.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include "qcepmacros.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QScriptValue>
#include <QPointer>
#include "ui_qxrdwindow.h"

#include "qxrdforwardtypes.h"
#include "qxrdimagequeue.h"
#include "qxrddataprocessor.h"

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

public:
  QxrdWindow(QxrdApplicationPtr app, QxrdAcquisitionPtr acq, QxrdDataProcessorPtr proc, QxrdAllocatorPtr alloc, QWidget *parent=0);
  virtual ~QxrdWindow();

public slots:
  void doAboutQxrd();
  void doOpenQXRDWebPage();
  void doSaveData();
  void doLoadData();
  void doSaveDark();
  void doLoadDark();
  void doSaveMask();
  void doLoadMask();
  void doSaveBadPixels();
  void doLoadBadPixels();
  void doSaveGainMap();
  void doLoadGainMap();
  void selectLogFile();
  void doRefineCenterTilt();

  void doAcquire();
  void doCancel();
  void onAcquireStarted(int dark);
  void onAcquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void onAcquireComplete(int dark);
  void doAcquireDark();
  void doCancelDark();

  void executeScript();
  void finishedCommand(QScriptValue result);
  void cancelScript();

  void doTest();

  QString timeStamp();
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

  void acquisitionReady();
  void acquisitionStarted();
  void acquisitionFinished();

  void darkAcquisitionStarted();

  void selectOutputDirectory();
  void clearStatusMessage();

  void enableTiltRefinement(bool enable);

signals:
  void executeCommand(QString cmd);

private slots:
  void newData();
  void newMask();
  void allocatedMemoryChanged();

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);
  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

  QxrdScriptEnginePtr scriptEngine() const;
  void setScriptEngine(QxrdScriptEnginePtr engine);
  QxrdDataProcessorPtr dataProcessor() const;

  void newDataAvailable(QxrdDoubleImageDataPtr img);
  void newMaskAvailable(QxrdMaskDataPtr img);

  QxrdDoubleImageDataPtr data();
  QxrdMaskDataPtr mask();

  QxrdAcquisitionPtr acquisition() const;

private:
  mutable QMutex                         m_Mutex;
  int                                    m_SettingsLoaded;
  QxrdApplicationPtr                     m_Application;
  QxrdAcquisitionPtr                     m_Acquisition;
  QxrdDataProcessorPtr                   m_DataProcessor;
  QxrdAllocatorPtr                       m_Allocator;
  QxrdScriptEnginePtr                    m_ScriptEngine;
  QxrdAcquireDialogPtr                   m_AcquireDialog;
  QxrdCenterFinderDialogPtr              m_CenterFinderDialog;
  QxrdIntegratorDialogPtr                m_IntegratorDialog;
  QxrdImageCalculatorPtr                 m_Calculator;
  QPointer<QxrdPowderFitDialog>          m_PowderFitDialog;
  QVector<double>                        m_Exposures;
  QProgressBarPtr                        m_Progress;
  QLabelPtr                              m_StatusMsg;
  QProgressBarPtr                        m_AllocationStatus;
  int                                    m_Acquiring;
  int                                    m_AcquiringDark;
  QTimer                                 m_StatusTimer;
  QTimer                                 m_UpdateTimer;

  QxrdDoubleImageDataPtr                 m_Data;
  QxrdDoubleImageDataPtr                 m_NewData;
  QAtomicInt                             m_NewDataAvailable;

  QxrdMaskDataPtr                        m_Mask;
  QxrdMaskDataPtr                        m_NewMask;
  QAtomicInt                             m_NewMaskAvailable;

  HEADER_IDENT("$Id: qxrdwindow.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdwindow.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.6  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.5  2010/09/09 19:57:31  jennings
*  Moved acquisition dialog into separate class
*  Reorganised acquisition dialog widgets to be more compact
*
*  Revision 1.1.2.4  2010/08/10 20:41:56  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*  Revision 1.1.2.3  2010/08/09 21:53:58  jennings
*  Made center and fit refiner into a modal dialog, partial implementation
*
*  Revision 1.1.2.2  2010/08/06 21:09:35  jennings
*  Initial partial implementation of powder ring fitting widget
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.64.2.9  2010/07/20 20:59:58  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.64.2.8  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.64.2.7  2010/06/23 21:39:34  jennings
*  Removed some unnecessary locking. Reduced frequency of display updates.
*
*  Revision 1.64.2.6  2010/05/06 18:52:02  jennings
*  Initialize m_NewData and m_NewMask to NULL
*
*  Revision 1.64.2.5  2010/04/26 23:46:14  jennings
*  *** empty log message ***
*
*  Revision 1.64.2.4  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.64.2.3  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.64.2.2  2010/04/19 21:48:27  jennings
*  More rearrangement
*
*  Revision 1.64.2.1  2010/04/18 17:20:48  jennings
*  Further updates
*
*  Revision 1.64  2010/04/09 22:29:38  jennings
*  Removed file browser, added CMake support, build under VC
*
*  Revision 1.63  2009/11/13 20:15:58  jennings
*  *** empty log message ***
*
*  Revision 1.62  2009/11/03 19:57:56  jennings
*  Added help menu with an about dialog and a link to the documentation web site
*
*  Revision 1.61  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*  Revision 1.60  2009/09/25 14:22:16  jennings
*  Simplified double-buffering for plotted data - there is now a separate copy of data and mask
*  in QxrdWindow
*
*  Revision 1.59  2009/09/21 16:27:58  jennings
*  Added user interface to log file path
*
*  Revision 1.58  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.57  2009/09/07 22:10:14  jennings
*  Allow NULL mask
*
*  Revision 1.56  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
*  Revision 1.55  2009/08/27 17:04:16  jennings
*  Added load/save commands for dark and mask
*
*  Revision 1.54  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.53  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.52  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.51  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.50  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.49  2009/07/16 21:26:25  jennings
*  Made various image display variables into properties
*
*  Revision 1.48  2009/07/16 20:10:43  jennings
*  Made various image display variables into properties
*
*  Revision 1.47  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.46  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.45  2009/06/30 21:36:17  jennings
*  Modified user interface to use tool box widgets
*
*  Revision 1.44  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.43  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

