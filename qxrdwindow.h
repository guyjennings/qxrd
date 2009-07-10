/******************************************************************
*
*  $Id: qxrdwindow.h,v 1.47 2009/07/10 22:54:23 jennings Exp $
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
class QxrdAcquisitionThread;
class QxrdAcquisition;
class QwtPlotSpectrogram;
class QCloseEvent;
class QxrdImageData;
//class QDirModel;
class QxrdDataProcessor;
class QxrdCenterFinder;
class QxrdCenterFinderDialog;
class QxrdIntegrator;
class QxrdIntegratorDialog;
class QxrdScriptEngine;
class QxrdSettings;

#include "qxrdimagequeue.h"

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

public:
  QxrdWindow(QxrdApplication *app, QxrdAcquisitionThread *acq, QWidget *parent=0);
  virtual ~QxrdWindow();

public:
  Q_PROPERTY(double displayMinimumPct     READ get_DisplayMinimumPct WRITE set_DisplayMinimumPct);
  QCEP_DOUBLE_PROPERTY(DisplayMinimumPct);

  Q_PROPERTY(double displayMaximumPct     READ get_DisplayMaximumPct WRITE set_DisplayMaximumPct);
  QCEP_DOUBLE_PROPERTY(DisplayMaximumPct);

  Q_PROPERTY(double displayMinimumVal     READ get_DisplayMinimumVal WRITE set_DisplayMinimumVal);
  QCEP_DOUBLE_PROPERTY(DisplayMinimumVal);

  Q_PROPERTY(double displayMaximumVal     READ get_DisplayMaximumVal WRITE set_DisplayMaximumVal);
  QCEP_DOUBLE_PROPERTY(DisplayMaximumVal);

  Q_PROPERTY(int displayScalingMode        READ get_DisplayScalingMode WRITE set_DisplayScalingMode);
  QCEP_INTEGER_PROPERTY(DisplayScalingMode);

  Q_PROPERTY(bool interpolatePixels        READ get_InterpolatePixels WRITE set_InterpolatePixels);
  QCEP_BOOLEAN_PROPERTY(InterpolatePixels);

  Q_PROPERTY(bool maintainAspectRatio        READ get_MaintainAspectRatio WRITE set_MaintainAspectRatio);
  QCEP_BOOLEAN_PROPERTY(MaintainAspectRatio);

  Q_PROPERTY(bool displayColorMap        READ get_DisplayColorMap WRITE set_DisplayColorMap);
  QCEP_BOOLEAN_PROPERTY(DisplayColorMap);

public slots:
  void doSaveData();
  void doLoadData();

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
  void criticalMessage(QString msg);

  void acquisitionReady();
  void acquisitionStarted();
  void acquisitionFinished();

  void darkAcquisitionStarted();

  void setReadoutTime(int n, double t);

  void selectOutputDirectory();
  void statusMessage(QString msg);
  void clearStatusMessage();

  void doLoadDarkImage();
  void doLoadBadPixels();
  void doLoadGainMap();

  void onToolBoxPageChanged(int);
  void onTabWidgetPageChanged(int);

signals:
  void executeCommand(QString cmd);

public:
  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);
  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

  QxrdImageData* loadNewImage(QString name);

  QxrdScriptEngine *scriptEngine() const;
  void setScriptEngine(QxrdScriptEngine *engine);

//  QxrdDataProcessor *processor() const;
  QxrdCenterFinder  *centerFinder() const;
  QxrdIntegrator    *integrator() const;

private:
  mutable QMutex          m_Mutex;
  int                     m_SettingsLoaded;
  QxrdApplication        *m_Application;
  QxrdAcquisitionThread  *m_AcquisitionThread;
  QxrdAcquisition        *m_Acquisition;
  QxrdScriptEngine       *m_ScriptEngine;
  QxrdCenterFinderDialog *m_CenterFinderDialog;
  QxrdCenterFinder       *m_CenterFinder;
  QxrdIntegratorDialog   *m_IntegratorDialog;
  QxrdIntegrator         *m_Integrator;
  QVector<double>         m_Exposures;
  QProgressBar           *m_Progress;
  QLabel                 *m_StatusMsg;
  int                     m_Acquiring;
  int                     m_AcquiringDark;
  QTimer                  m_StatusTimer;
  HEADER_IDENT("$Id: qxrdwindow.h,v 1.47 2009/07/10 22:54:23 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdwindow.h,v $
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

