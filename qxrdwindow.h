/******************************************************************
*
*  $Id: qxrdwindow.h,v 1.43 2009/06/27 22:50:33 jennings Exp $
*
*******************************************************************/

#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include "qcepmacros.h"

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
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

#include "qxrdimagequeue.h"

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

public:
  QxrdWindow(QxrdApplication *app, QxrdAcquisitionThread *acq, QWidget *parent=0);
  virtual ~QxrdWindow();

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

  void onAcquisitionRunning();

  void doAcquire();
  void doCancel();
  void onAcquireStarted(int dark);
  void onAcquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void onAcquireComplete(int dark);
  void doAcquireDark();
  void doCancelDark();
  void executeScript();

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
  QString darkImagePath();
  void setDarkImagePath(QString path);

  int performBadPixels();
  void setPerformBadPixels(int corr);
  void doLoadBadPixels();
  void loadBadPixels(QString name);
  QString badPixelsPath();
  void setBadPixelsPath(QString path);

  int performGainCorrection();
  void setPerformGainCorrection(int corr);
  void doLoadGainMap();
  void loadGainMap(QString name);
  QString gainMapPath();
  void setGainMapPath(QString path);

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

private:
  int                     m_SettingsLoaded;
  QxrdApplication        *m_Application;
  QxrdAcquisitionThread  *m_AcquisitionThread;
  QxrdAcquisition        *m_Acquisition;
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
  HEADER_IDENT("$Id: qxrdwindow.h,v 1.43 2009/06/27 22:50:33 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdwindow.h,v $
*  Revision 1.43  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

