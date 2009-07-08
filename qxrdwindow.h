/******************************************************************
*
*  $Id: qxrdwindow.h,v 1.46 2009/07/08 19:06:27 jennings Exp $
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

#include "qxrdimagequeue.h"

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

public:
  QxrdWindow(QxrdApplication *app, QxrdAcquisitionThread *acq, QWidget *parent=0);
  virtual ~QxrdWindow();

public:
  Q_PROPERTY(QString darkImagePath READ get_DarkImagePath WRITE set_DarkImagePath);
  QCEP_STRING_PROPERTY(DarkImagePath);

  Q_PROPERTY(QString badPixelsPath READ get_BadPixelsPath WRITE set_BadPixelsPath);
  QCEP_STRING_PROPERTY(BadPixelsPath);

  Q_PROPERTY(QString gainMapPath READ get_GainMapPath WRITE set_GainMapPath);
  QCEP_STRING_PROPERTY(GainMapPath);

public slots:
  void doSaveData();
  void doLoadData();
//  void doImportData();
  void loadData(QString name);
//  void importData(QString name);
  void saveData(QString name);
  void saveImageData(QxrdImageData *image);
  void saveRawData(QxrdImageData *image);
  void saveNamedImageData(QString name, QxrdImageData *image);

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

  int performDarkSubtraction();
  void setPerformDarkSubtraction(int subt);
  int saveRawImages();
  void setSaveRawImages(int sav);
  void doLoadDarkImage();
  void loadDarkImage(QString name);
//  QString darkImagePath();
//  void setDarkImagePath(QString path);

  int performBadPixels();
  void setPerformBadPixels(int corr);
  void doLoadBadPixels();
  void loadBadPixels(QString name);
//  QString badPixelsPath();
//  void setBadPixelsPath(QString path);

  int performGainCorrection();
  void setPerformGainCorrection(int corr);
  void doLoadGainMap();
  void loadGainMap(QString name);
//  QString gainMapPath();
//  void setGainMapPath(QString path);

  double displayMinimumPct();
  void setDisplayMinimumPct(double pct);
  double displayMaximumPct();
  void setDisplayMaximumPct(double pct);
  int interpolatePixels();
  void setInterpolatePixels(int interp);
  int maintainAspectRatio();
  void setMaintainAspectRatio(int prsrv);

  void onProcessedImageAvailable();
  void onDarkImageAvailable();

  void showMaskRange();
  void hideMaskRange();
  void showMaskAll();
  void hideMaskAll();

  void onToolBoxPageChanged(int);
  void onTabWidgetPageChanged(int);

signals:
  void executeCommand(QString cmd);

public:
  void readSettings();
  void writeSettings();
  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

  QxrdImageData* loadNewImage(QString name);

  void darkImageAcquired(QxrdImageData *image);
  void newData(QxrdImageData *image);
  void newDarkImage(QxrdImageData *image);
  void newBadPixelsImage(QxrdImageData *image);
  void newGainMapImage(QxrdImageData *image);

  QxrdImageData* data();
  QxrdImageData* darkImage();

  QxrdScriptEngine *scriptEngine() const;
  void setScriptEngine(QxrdScriptEngine *engine);

  QxrdDataProcessor *processor() const;
  QxrdCenterFinder  *centerFinder() const;
  QxrdIntegrator    *integrator() const;

private:
  mutable QMutex          m_Mutex;
  int                     m_SettingsLoaded;
  QxrdApplication        *m_Application;
  QxrdAcquisitionThread  *m_AcquisitionThread;
  QxrdAcquisition        *m_Acquisition;
  QxrdScriptEngine       *m_ScriptEngine;
  QxrdDataProcessor      *m_DataProcessor;
  QxrdCenterFinderDialog *m_CenterFinderDialog;
  QxrdCenterFinder       *m_CenterFinder;
  QxrdIntegratorDialog   *m_IntegratorDialog;
  QxrdIntegrator         *m_Integrator;
  QVector<double>         m_Exposures;
  QProgressBar           *m_Progress;
  QLabel                 *m_StatusMsg;
  int                     m_Acquiring;
  int                     m_AcquiringDark;
  //  QxrdImageQueue          m_AcquiredImages;
  QxrdImageData          *m_Data;
  QxrdImageData          *m_DarkFrame;
  QxrdImageData          *m_BadPixels;
  QxrdImageData          *m_GainFrame;
  QTimer                  m_StatusTimer;
//  QStringList             m_StatusMessages;
//
//  QDirModel              *m_FileBrowserModel;
//  QTimer                  m_FileBrowserTimer;
  HEADER_IDENT("$Id: qxrdwindow.h,v 1.46 2009/07/08 19:06:27 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdwindow.h,v $
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

