#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QTimer>
#include <QPointer>
#include <QScriptValue>

#include "ui_qxrdwindow.h"

class QxrdApplication;
class QxrdExperiment;
class QxrdAcquisition;
class QxrdDataProcessor;
class QxrdAllocator;
class QxrdScriptEngine;
class QxrdAcquireDialogBase;
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
  Q_OBJECT

public:
  QxrdWindow(QxrdSettingsSaver *saver,
             QxrdApplication *app,
             QxrdExperiment *doc,
             QxrdAcquisition *acq,
             QxrdDataProcessor *proc,
             QxrdAllocator *alloc,
             QSettings *settings,
             QString secion,
             QWidget *parent=0);
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
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);
  void possiblyClose();
  bool wantToClose();
  void closeEvent (QCloseEvent * event);

  QxrdDataProcessor *dataProcessor() const;

  void newDataAvailable(QxrdDoubleImageDataPtr img, QxrdMaskDataPtr overflow);
  void newMaskAvailable(QxrdMaskDataPtr img);

  QxrdDoubleImageDataPtr data();
  QxrdMaskDataPtr mask();

  QxrdAcquisition *acquisition() const;

  void captureSize();
  void resizeEvent(QResizeEvent *);
  void moveEvent(QMoveEvent *);

private:
  void shrinkDockWidget(QDockWidget *dockWidget);
  void shrinkObject(QObject *obj);

  void setupRecentExperimentsMenu(QAction *action);

public:
  Q_PROPERTY(QByteArray windowGeometry READ get_WindowGeometry WRITE set_WindowGeometry)
  QCEP_BYTE_ARRAY_PROPERTY(WindowGeometry)

  Q_PROPERTY(QByteArray windowState READ get_WindowState WRITE set_WindowState)
  QCEP_BYTE_ARRAY_PROPERTY(WindowState)

private:
  mutable QMutex                         m_Mutex;
  int                                    m_SettingsLoaded;
  QxrdApplication                       *m_Application;
  QxrdSettingsSaver                     *m_Saver;
  QxrdExperiment                        *m_Experiment;
  QxrdAcquisition                       *m_Acquisition;
  QxrdDataProcessor                     *m_DataProcessor;
  QxrdAllocator                         *m_Allocator;
  QxrdAcquireDialogBase                 *m_AcquireDialog;
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

typedef QSharedPointer<QxrdWindow> QxrdWindowPtr;

#endif
