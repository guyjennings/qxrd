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
#include "qxrdimagedisplaywidget.h"

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

  void newDataAvailable(QxrdDoubleImageDataPtr img, QxrdMaskDataPtr overflow);
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
  QxrdMaskDataPtr                        m_Overflow;
  QxrdDoubleImageDataPtr                 m_NewData;
  QxrdMaskDataPtr                        m_NewOverflow;
  QAtomicInt                             m_NewDataAvailable;

  QxrdMaskDataPtr                        m_Mask;
  QxrdMaskDataPtr                        m_NewMask;
  QAtomicInt                             m_NewMaskAvailable;

  QxrdImageDisplayWidget                *m_ImageDisplay;
};

#endif
