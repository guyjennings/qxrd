#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QPointer>
#include <QScriptValue>

#include "ui_qxrdwindow.h"

#include "qxrdmainwindow.h"
#include "qcepallocator.h"
#include "qxrdscriptengine.h"
#include "qxrdacquisitiondialog.h"
#include "qxrdacquisitionscalerdialog-ptr.h"
#include "qxrddisplaydialog.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcorrectiondialog.h"
#include "qxrdintegratordialog.h"
#include "qxrdintegratorplot.h"
#include "qxrdcalibrantdialog.h"
#include "qcepdatasetbrowserdialog.h"
#include "qxrdimagecalculator.h"
#include "qxrdfilebrowser.h"
#include "qxrdslicedialog.h"
#include "qxrdhistogramdialog.h"
#include "qxrdinfodialog.h"
#include "qxrdscriptdialog.h"
#include "qxrdimagedisplaywidget.h"
#include "qxrdhighlighter.h"
#include "qxrdmaskdialog.h"
#include "qxrddistortioncorrectiondialog.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdsynchronizedacquisitiondialog-ptr.h"
#include "qxrdacquisitionextrainputsdialog-ptr.h"
#include "qxrdwindowsettings.h"
#include "qcepobjectnamer.h"

class QxrdWindow : public QxrdMainWindow, public Ui::QxrdWindow, public QEnableSharedFromThis<QxrdWindow>
{
  Q_OBJECT

public:
  QxrdWindow(QxrdWindowSettingsWPtr settings,
             QxrdApplicationWPtr appl,
             QxrdExperimentWPtr docw,
             QxrdAcquisitionWPtr acqw,
             QxrdDataProcessorWPtr procw,
             QcepAllocatorWPtr allocw,
             QWidget *parent);
  virtual ~QxrdWindow();
  void initialize();

  void onAcquisitionInit();

public slots:
  void saveExperimentCopy();
  void saveExperimentAs();
  void doEditPreferences();
  void doEditDetectorPreferences();
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
//  void doOpenAcquisitionWindow();

//  void doPolarTransform();
//  void doPolarNormalization();

  void updateTitle();
  void acquireStarted();
  void acquiredFrame(QString fileName, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireComplete();

  void crashProgram();
  void testWidget();

  void executeScript();
  void finishedCommand(QScriptValue result);
  void cancelScript();
  void doLoadScript();
  void loadScript(QString path);

  void doTest();

  QString timeStamp() const;
  void warningMessage(QString msg);
  void displayMessage(QString msg);
  void displayStatusMessage(QString msg);
  void displayCriticalMessage(QString msg);
  void initialLogEntry(QString aline);
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

//  void selectOutputDirectory();
  void clearStatusMessage();

  void enableTiltRefinement(bool enable);

signals:
  void executeCommand(QString cmd);

private slots:
  void newData();
  void newMask();
  void allocatedMemoryChanged();
  void integrationXUnitsChanged(int newXUnits);
  void onMessageWindowLinesChanged(int newVal);
  void onUpdateIntervalMsecChanged(int newVal);
  void populateExperimentsMenu();
  void populateRecentExperimentsMenu();
  void populateConfigureDetectorMenu();
  void populateDetectorControlWindowsMenu();
  void plotPowderRingRadii();
  void plotPowderRingTwoTheta();
  void plotPowderRingCenters();

public:
  void closeEvent (QCloseEvent * event);

  QxrdDataProcessorWPtr dataProcessor() const;

  void newDataAvailable(QcepDoubleImageDataPtr img, QcepMaskDataPtr overflow);
  void newMaskAvailable(QcepMaskDataPtr img);

  QcepDoubleImageDataPtr data();
  QcepMaskDataPtr mask();

  QxrdAcquisitionWPtr acquisition() const;

  void captureSize();
  void resizeEvent(QResizeEvent *);
  void moveEvent(QMoveEvent *);

private:
  void setupRecentExperimentsMenu(QAction *action);

private:
  QcepObjectNamer                        m_ObjectNamer;
  mutable QMutex                         m_Mutex;
  QxrdWindowSettingsWPtr                 m_WindowSettings;
  QxrdApplicationWPtr                    m_Application;
  QxrdExperimentWPtr                     m_Experiment;
  QxrdAcquisitionWPtr                    m_Acquisition;
  QxrdDataProcessorWPtr                  m_DataProcessor;
  QcepAllocatorWPtr                      m_Allocator;
  QxrdAcquisitionDialog                 *m_AcquisitionDialog;
  QxrdAcquisitionScalerDialog           *m_AcquisitionScalerDialog;
  QxrdAcquisitionExtraInputsDialogPtr    m_AcquisitionExtraInputsDialog;
  QxrdSynchronizedAcquisitionDialogPtr   m_SynchronizedAcquisitionDialog;
  QxrdDisplayDialog                     *m_DisplayDialog;
  QxrdCenterFinderDialog                *m_CenterFinderDialog;
  QxrdMaskDialog                        *m_MaskDialog;
  QxrdCorrectionDialog                  *m_CorrectionDialog;
  QxrdCalibrantDialog                   *m_CalibrantDialog;
  QcepDatasetBrowserDialog              *m_DatasetBrowserDialog;
  QxrdIntegratorDialog                  *m_IntegratorDialog;
  QxrdImageCalculator                   *m_Calculator;
  QxrdInputFileBrowser                  *m_InputFileBrowser;
  QxrdOutputFileBrowser                 *m_OutputFileBrowser;
  QxrdSliceDialog                       *m_SliceDialog;
  QxrdHistogramDialog                   *m_HistogramDialog;
  QxrdInfoDialog                        *m_ImageInfoDialog;
  QxrdScriptDialog                      *m_ScriptDialog;
  QxrdDistortionCorrectionDialog        *m_DistortionCorrectionDialog;
  QVector<double>                        m_Exposures;
  QProgressBar                          *m_Progress;
  QLabel                                *m_StatusMsg;
  QProgressBar                          *m_AllocationStatus;
  QTimer                                 m_StatusTimer;
  QTimer                                 m_UpdateTimer;

  QcepDoubleImageDataPtr                 m_Data;
  QcepMaskDataPtr                        m_Overflow;
  QcepDoubleImageDataPtr                 m_NewData;
  QcepMaskDataPtr                        m_NewOverflow;
  QAtomicInt                             m_NewDataAvailable;

  QcepMaskDataPtr                        m_Mask;
  QcepMaskDataPtr                        m_NewMask;
  QAtomicInt                             m_NewMaskAvailable;

  QxrdImageDisplayWidget                *m_ImageDisplay;
  QxrdHighlighter                       *m_Highlighter;

  QString                                m_ScriptPath;

  QMenu                                 *m_RecentExperimentsMenu;

  QString                                m_CachedGeometryPath;
  QString                                m_CachedIntensityPath;
};

#endif
