#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include "ui_qxrdwindow.h"

class QxrdApplication;
class QxrdAcquisitionThread;
class QwtPlotSpectrogram;
class QCloseEvent;
class QxrdImageData;
class QDirModel;
class QxrdDataProcessor;

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
  void doImportData();
  void loadData(QString name);
  void importData(QString name);
  void saveData(QString name);
  void saveImageData(QxrdImageData *image);
  void saveRawData(QxrdImageData *image);
  void saveNamedImageData(QString name, QxrdImageData *image);

  void doAcquire();
  void doCancel();
  void acquireComplete();
  void doAcquireDark();
  void doCancelDark();
  void acquireDarkComplete();

  void printMessage(QString msg);

  void acquisitionReady();
  void acquisitionStarted();
  void acquisitionFinished();

  void darkAcquisitionStarted();

  void setIntegrationTime(int n, double t);

  void setExposureTime(double t);
  void setIntegrationMode(int mode);
  void setNSummed(int nsummed);
  void setNFrames(int nframes);
  void setFileIndex(int index);
  void setFilePattern(QString pattern);
  void setOutputDirectory(QString path);
  void setDarkNSummed(int nsummed);

  void selectOutputDirectory();
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void statusMessage(QString msg);

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

  void setFileBrowserDirectory(QString dir);
  void refreshFileBrowser();

 public:
  double  exposureTime();
  int     integrationMode();
  int     nSummed();
  int     nFrames();
  int     fileIndex();
  QString filePattern();
  QString outputDirectory();
  int     darkNSummed();

  int acquisitionStatus(double time);

  void readSettings();
  void saveSettings();
  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

//  void enqueue(QxrdImageData *image);
//  QxrdImageData* dequeue();
//  QxrdImageData* nextAvailableImage();
//  void returnImageToPool(QxrdImageData *img);
  QxrdImageData* loadNewImage(QString name);

  void darkImageAcquired(QxrdImageData *image);
  void newData(QxrdImageData *image);
  void newDarkImage(QxrdImageData *image);
  void newBadPixelsImage(QxrdImageData *image);
  void newGainMapImage(QxrdImageData *image);

  QxrdImageData* darkImage();

 private:
  int                     m_SettingsLoaded;
  QxrdApplication        *m_Application;
  QxrdAcquisitionThread  *m_AcquisitionThread;
  QxrdDataProcessor      *m_DataProcessor;
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

  QDirModel              *m_FileBrowserModel;
  QTimer                  m_FileBrowserTimer;
};

#endif
