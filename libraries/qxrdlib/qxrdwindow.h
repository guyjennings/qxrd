#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include "qxrdlib_global.h"
#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QPointer>
#include <QScriptValue>
#include <QJSValue>

#include "ui_qxrdwindow.h"

#include "qxrdmainwindow.h"
#include "qxrdscriptengine.h"
#include "qxrdacquisitionscalerdialog-ptr.h"
#include "qxrdplotwidgetdialog.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcorrectiondialog.h"
#include "qxrdintegratordialog.h"
#include "qxrdintegratorplot.h"
#include "qcepdatasetbrowserdialog.h"
#include "qxrdimagecalculator.h"
#include "qxrdfilebrowser.h"
#include "qxrdslicedialog.h"
#include "qxrdhistogramdialog.h"
#include "qxrdinfodialog.h"
#include "qxrdscriptdialog.h"
#include "qxrdhighlighter.h"
#include "qxrdmaskdialog.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdsynchronizedacquisitiondialog-ptr.h"
#include "qxrdacquisitionextrainputsdialog-ptr.h"
#include "qxrdwindowsettings-ptr.h"
#include "qcepobjectnamer.h"

#define QXRD_WINDOW_STATE_VERSION 3

class QXRD_EXPORT QxrdWindow : public QxrdMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  QxrdWindow(QxrdWindowSettingsWPtr settings,
             QxrdAppCommonWPtr appl,
             QxrdExperimentWPtr docw,
             QxrdAcqCommonWPtr acqw,
             QxrdProcessorWPtr procw);
  virtual ~QxrdWindow();
  //TODO: change to QcepObjectWPtr
  void initialize(QcepObjectWPtr parent);
//  void setupMenus();

  void onAcquisitionInit();

public slots:
  void doSaveExperimentAsText();
  void doSaveData();
  void doLoadData();
  void doSaveDark();
  void doLoadDark();
  void doClearDark();
  void doSaveMask();
  void doLoadMask();
  void doClearMask();
  void doSaveBadPixels();
  void doLoadBadPixels();
  void doClearBadPixels();
  void doSaveGainMap();
  void doLoadGainMap();
  void doClearGainMap();
  void doSaveCachedGeometry();
  void doSaveCachedIntensity();
//  void selectLogFile();
  void doRefineCenterTilt();
  void doAccumulateImages();
  void doAddImages();
  void doSubtractImages();
  void doIntegrateSequence();
  void doProcessSequence();
  void doProjectAlongX();
  void doProjectAlongY();
  void doProjectAlongZ();
  void doCorrelate();
  void doReflectVertically();
  void doReflectHorizontally();

  void doNewTestImageGenerator();
  void doNewTestScanGenerator();

//  void updateTitle();
//  void acquireStarted();
//  void acquiredFrame(QString fileName, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
//  void acquireComplete();

  void executeScript();
  void executeScriptJS();
  void finishedCommand(QScriptValue result);
  void finishedCommandJS(QJSValue result);
  void cancelScript();
  void doLoadScript();
  void loadScript(QString path);

  QString timeStamp() const;
  void warningMessage(QString msg);
  void displayMessage(QString msg);
  void displayCriticalMessage(QString msg);
  void initialLogEntry(QString aline);
  virtual void printLine(QString line);
  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

//  void selectOutputDirectory();

  void enableTiltRefinement(bool enable);

signals:
  void executeCommand(QString cmd);
  void executeCommandJS(QString cmd);

private slots:
  void doTimerUpdate();
  void newMask();
  void integrationXUnitsChanged(int newXUnits);
  void onMessageWindowLinesChanged(int newVal);
//  void populateWindowMenu();
  void populateConfigureDetectorMenu();
  void populateDetectorControlWindowsMenu();
  void plotPowderRingRadii();
  void plotPowderRingTwoTheta();
  void plotPowderRingCenters();

public:
  void closeEvent (QCloseEvent * event);

  QxrdProcessorWPtr processor() const;

  void newDataAvailable(QcepDoubleImageDataPtr img, QcepMaskDataPtr overflow);
  void newMaskAvailable(QcepMaskDataPtr img);

  QcepDoubleImageDataPtr data();
  QcepMaskDataPtr mask();

  void captureSize();
  void resizeEvent(QResizeEvent *);
  void moveEvent(QMoveEvent *);

private:
  QcepObjectNamer                        m_ObjectNamer;
  mutable QMutex                         m_Mutex;
  QxrdWindowSettingsWPtr                 m_WindowSettings;
//  QxrdApplicationWPtr                    m_Application;
//  QxrdExperimentWPtr                     m_Experiment;
//  QxrdAcquisitionWPtr                    m_Acquisition;
//  QxrdProcessorWPtr                      m_Processor;
//  QxrdAcquisitionScalerDialog           *m_AcquisitionScalerDialog;
//  QxrdAcquisitionExtraInputsDialogPtr    m_AcquisitionExtraInputsDialog;
//  QxrdSynchronizedAcquisitionDialogPtr   m_SynchronizedAcquisitionDialog;
//  QxrdCenterFinderDialog                *m_CenterFinderDialog;
//  QxrdMaskDialog                        *m_MaskDialog;
//  QxrdCorrectionDialog                  *m_CorrectionDialog;
//  QcepDatasetBrowserDialog              *m_DatasetBrowserDialog;
//  QxrdIntegratorDialog                  *m_IntegratorDialog;
//  QxrdImageCalculator                   *m_Calculator;
//  QxrdInputFileBrowser                  *m_InputFileBrowser;
//  QxrdOutputFileBrowser                 *m_OutputFileBrowser;
//  QxrdSliceDialog                       *m_SliceDialog;
//  QxrdHistogramDialog                   *m_HistogramDialog;
//  QxrdInfoDialog                        *m_ImageInfoDialog;
//  QxrdScriptDialog                      *m_ScriptDialog;
  QVector<double>                        m_Exposures;

  QcepDoubleImageDataPtr                 m_Data;
  QcepMaskDataPtr                        m_Overflow;
  QcepDoubleImageDataPtr                 m_NewData;
  QcepMaskDataPtr                        m_NewOverflow;
  QAtomicInt                             m_NewDataAvailable;

  QcepMaskDataPtr                        m_Mask;
  QcepMaskDataPtr                        m_NewMask;
  QAtomicInt                             m_NewMaskAvailable;

  QxrdHighlighter                       *m_Highlighter;

  QString                                m_ScriptPath;

  QMenu                                 *m_RecentExperimentsMenu;

  QString                                m_CachedGeometryPath;
  QString                                m_CachedIntensityPath;
};

#endif
