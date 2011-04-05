#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QPointer>
#include <QScriptValue>

#include "ui_qxrdwindow.h"

class QxrdApplication;
class QxrdAcquisition;
class QxrdDataProcessor;
class QxrdAllocator;
class QxrdScriptEngine;
class QxrdAcquireDialog;
class QxrdSynchronizedAcquisitionDialog;
class QxrdDisplayDialog;
class QxrdCenterFinderDialog;
class QxrdMaskDialog;
class QxrdCorrectionDialog;
class QxrdIntegratorDialog;
class QxrdImageCalculator;
class QxrdInputFileBrowser;
class QxrdOutputFileBrowser;
class QxrdSliceDialog;
class QxrdHistogramDialog;
class QxrdInfoDialog;
class QxrdPowderFitDialog;
class QxrdImageDisplayWidget;
class QxrdHighlighter;

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

public:
  QxrdWindow(QxrdApplication *app, QxrdAcquisition *acq, QxrdDataProcessor *proc, QxrdAllocator *alloc, QWidget *parent=0);
  virtual ~QxrdWindow();
  void onAcquisitionInit();

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
  void doAccumulateImages();
  void doIntegrateSequence();
  void doProcessSequence();

  void doAcquire();
  void doCancel();
  void acquireStarted();
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireComplete();
  void doAcquireDark();

  void executeScript();
  void finishedCommand(QScriptValue result);
  void cancelScript();

  void doTest();

  QString timeStamp();
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

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

  QxrdScriptEngine *scriptEngine() const;
  void setScriptEngine(QxrdScriptEngine *engine);
  QxrdDataProcessor *dataProcessor() const;

  void newDataAvailable(QxrdDoubleImageDataPtr img, QxrdMaskDataPtr overflow);
  void newMaskAvailable(QxrdMaskDataPtr img);

  QxrdDoubleImageDataPtr data();
  QxrdMaskDataPtr mask();

  QxrdAcquisition *acquisition() const;

private:
  void shrinkDockWidget(QDockWidget *dockWidget);
  void shrinkObject(QObject *obj);

private:
  mutable QMutex                         m_Mutex;
  int                                    m_SettingsLoaded;
  QxrdApplication                       *m_Application;
  QxrdAcquisition                       *m_Acquisition;
  QxrdDataProcessor                     *m_DataProcessor;
  QxrdAllocator                         *m_Allocator;
  QxrdScriptEngine                      *m_ScriptEngine;
  QxrdAcquireDialog                     *m_AcquireDialog;
  QxrdSynchronizedAcquisitionDialog     *m_SynchronizedAcquisitionDialog;
  QxrdDisplayDialog                     *m_DisplayDialog;
  QxrdCenterFinderDialog                *m_CenterFinderDialog;
  QxrdMaskDialog                        *m_MaskDialog;
  QxrdCorrectionDialog                  *m_CorrectionDialog;
  QxrdIntegratorDialog                  *m_IntegratorDialog;
  QxrdImageCalculator                   *m_Calculator;
  QxrdInputFileBrowser                  *m_InputFileBrowser;
  QxrdOutputFileBrowser                 *m_OutputFileBrowser;
  QxrdSliceDialog                       *m_SliceDialog;
  QxrdHistogramDialog                   *m_HistogramDialog;
  QxrdInfoDialog                        *m_ImageInfoDialog;
  QPointer<QxrdPowderFitDialog>          m_PowderFitDialog;
  QVector<double>                        m_Exposures;
  QProgressBar                          *m_Progress;
  QLabel                                *m_StatusMsg;
  QProgressBar                          *m_AllocationStatus;
  int                                    m_Acquiring;
  int                                    m_AcquiringDark;
  QTimer                                 m_StatusTimer;
  QTimer                                 m_UpdateTimer;

  QxrdDoubleImageDataPtr                 m_Data;
  QxrdMaskDataPtr                        m_Overflow;
  QxrdDoubleImageDataPtr                 m_NewData;
  QxrdMaskDataPtr                        m_NewOverflow;
  QAtomicInt                             m_NewDataAvailable;

  QxrdMaskDataPtr                        m_Mask;
  QxrdMaskDataPtr                        m_NewMask;
  QAtomicInt                             m_NewMaskAvailable;

  QxrdImageDisplayWidget                *m_ImageDisplay;
  QxrdHighlighter                       *m_Highlighter;
};

#endif
