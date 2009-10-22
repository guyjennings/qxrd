/******************************************************************
*
*  $Id: qxrdwindow.h,v 1.61 2009/10/22 19:22:28 jennings Exp $
*
*******************************************************************/

#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include "qcepmacros.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QScriptValue>
#include "ui_qxrdwindow.h"

class QxrdApplication;
class QxrdAcquisition;
class QxrdDataProcessor;
class QwtPlotSpectrogram;
class QCloseEvent;
template <typename T> class QxrdImageData;
class QxrdMaskData;
//class QDirModel;
class QxrdDataProcessor;
class QxrdCenterFinder;
class QxrdCenterFinderDialog;
class QxrdIntegrator;
class QxrdIntegratorDialog;
class QxrdScriptEngine;
class QxrdSettings;
class QxrdFileBrowser;

#include "qxrdimagequeue.h"

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

public:
  QxrdWindow(QxrdApplication *app, QxrdAcquisition *acq, QxrdDataProcessor *proc, QWidget *parent=0);
  virtual ~QxrdWindow();

public slots:
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

  void setReadoutTime(int n, double t);

  void selectOutputDirectory();
  void clearStatusMessage();

signals:
  void executeCommand(QString cmd);

private slots:
  void newData();
  void newMask();

public:
  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);
  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

//  QxrdImageData* loadNewImage(QString name);

  QxrdScriptEngine *scriptEngine() const;
  void setScriptEngine(QxrdScriptEngine *engine);
  QxrdDataProcessor *dataProcessor() const;

  void newDataAvailable(QxrdDoubleImageData *img);
  void newMaskAvailable(QxrdMaskData *img);

  QxrdDoubleImageData *data();
  QxrdMaskData *mask();

//  QxrdDataProcessor *processor() const;

private:
  mutable QMutex          m_Mutex;
  int                     m_SettingsLoaded;
  QxrdApplication        *m_Application;
  QxrdAcquisition        *m_Acquisition;
  QxrdDataProcessor      *m_DataProcessor;
  QxrdScriptEngine       *m_ScriptEngine;
  QxrdCenterFinderDialog *m_CenterFinderDialog;
//  QxrdCenterFinder       *m_CenterFinder;
  QxrdIntegratorDialog   *m_IntegratorDialog;
//  QxrdIntegrator         *m_Integrator;
  QxrdFileBrowser        *m_FileBrowser;
  QVector<double>         m_Exposures;
  QProgressBar           *m_Progress;
  QLabel                 *m_StatusMsg;
  int                     m_Acquiring;
  int                     m_AcquiringDark;
  QTimer                  m_StatusTimer;

  mutable QMutex          m_NewDataMutex;
  QxrdDoubleImageData    *m_Data;
  QxrdDoubleImageData    *m_NewData;
  int                     m_NewDataAvailable;

  mutable QMutex          m_NewMaskMutex;
  QxrdMaskData           *m_Mask;
  QxrdMaskData           *m_NewMask;
  int                     m_NewMaskAvailable;

//  QAtomicInt              m_Plotting;
//  QAtomicInt              m_SpareDataAvailable;

  HEADER_IDENT("$Id: qxrdwindow.h,v 1.61 2009/10/22 19:22:28 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdwindow.h,v $
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

