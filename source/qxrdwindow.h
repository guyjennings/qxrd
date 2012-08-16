#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QPointer>
#include <QScriptValue>

#include "ui_qxrdwindow.h"

#include "qxrdallocator.h"
#include "qxrdscriptengine.h"
#include "qxrdacquisitiondialog.h"

#include "qxrddisplaydialog.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcorrectiondialog.h"
#include "qxrdintegratordialog.h"
#include "qxrdintegratorplot.h"
#include "qxrdimagecalculator.h"
#include "qxrdfilebrowser.h"
#include "qxrdslicedialog.h"
#include "qxrdhistogramdialog.h"
#include "qxrdinfodialog.h"
#include "qxrdpowderfitdialog.h"
#include "qxrdimagedisplaywidget.h"
#include "qxrdhighlighter.h"
#include "qxrdmaskdialog.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdsynchronizedacquisitiondialog-ptr.h"
#include "qxrdacquisitionextrainputsdialog-ptr.h"
#include "qxrdwindowsettings.h"
#include "qxrdobjectnamer.h"

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT

public:
  QxrdWindow(QxrdWindowSettingsWPtr settings,
             QxrdApplication *app,
             QxrdExperimentWPtr docw,
             QxrdAcquisitionWPtr acqw,
             QxrdDataProcessorWPtr procw,
             QxrdAllocatorWPtr allocw,
             QWidget *parent);
  virtual ~QxrdWindow();
  void onAcquisitionInit();

public slots:
  void saveExperimentCopy();
  void saveExperimentAs();
  void doEditPreferences();
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
//  void selectLogFile();
  void doRefineCenterTilt();
  void doAccumulateImages();
  void doIntegrateSequence();
  void doProcessSequence();

  void updateTitle();
  void acquireStarted();
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireComplete();

  void crashProgram();
  void testWidget();
  void shrinkPanels(int fontSize, int spacing);

  void executeScript();
  void finishedCommand(QScriptValue result);
  void cancelScript();
  void doLoadScript();
  void loadScript(QString path);

  void doTest();

  QString timeStamp();
  void warningMessage(QString msg);
  void displayMessage(QString msg);
  void displayStatusMessage(QString msg);
  void displayCriticalMessage(QString msg);

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

public:
  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

  QxrdDataProcessorWPtr dataProcessor() const;

  void newDataAvailable(QxrdDoubleImageDataPtr img, QxrdMaskDataPtr overflow);
  void newMaskAvailable(QxrdMaskDataPtr img);

  QxrdDoubleImageDataPtr data();
  QxrdMaskDataPtr mask();

  QxrdAcquisitionWPtr acquisition() const;

  void captureSize();
  void resizeEvent(QResizeEvent *);
  void moveEvent(QMoveEvent *);

private:
  void shrinkDockWidget(QDockWidget *dockWidget, int fontSize, int spacing);
  void shrinkObject(QObject *obj, int fontSize, int spacing);

  void setupRecentExperimentsMenu(QAction *action);

private:
  QxrdObjectNamer                        m_ObjectNamer;
  mutable QMutex                         m_Mutex;
  QxrdWindowSettingsWPtr                 m_WindowSettings;
  QxrdApplication                       *m_Application;
  QxrdExperimentWPtr                     m_Experiment;
  QxrdAcquisitionWPtr                    m_Acquisition;
  QxrdDataProcessorWPtr                  m_DataProcessor;
  QxrdAllocatorWPtr                      m_Allocator;
  QxrdAcquisitionDialog                 *m_AcquisitionDialog;
  QxrdAcquisitionExtraInputsDialogPtr    m_AcquisitionExtraInputsDialog;
  QxrdSynchronizedAcquisitionDialogPtr   m_SynchronizedAcquisitionDialog;
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

  QString                                m_ScriptPath;

  QMenu                                 *m_RecentExperimentsMenu;
};

#endif
