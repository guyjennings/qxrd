#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrdmainwindow.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdmainwindowsettings.h"
#include "qxrdexperimentpreferencesdialog.h"
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFileDialog>
#include "qxrdapplicationsettings.h"
#include "qcepallocator.h"

QxrdMainWindow::QxrdMainWindow(QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acqw, QxrdProcessorWPtr procw)
  : QcepMainWindow(),
    m_Name(name),
    m_Application(app),
    m_Experiment(expt),
    m_Acquisition(acqw),
    m_DataProcessor(procw),
    m_Progress(NULL),
    m_FileMenuP(NULL),
    m_EditMenuP(NULL),
    m_WindowMenuP(NULL)
{
}

void QxrdMainWindow::setupMenus(QMenu *file, QMenu *edit, QMenu *window)
{
  m_StatusMsg = new QLabel(NULL);
  m_StatusMsg -> setMinimumWidth(200);
  m_StatusMsg -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_StatusMsg -> setToolTip(tr("Status Messages"));

  statusBar() -> addPermanentWidget(m_StatusMsg);

  m_Progress = new QProgressBar(NULL);
  m_Progress -> setMinimumWidth(150);
  m_Progress -> setMinimum(0);
  m_Progress -> setMaximum(100);
  m_Progress -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_Progress -> setToolTip(tr("Acquisition progress"));

  statusBar() -> addPermanentWidget(m_Progress);

  m_AllocationStatus = new QProgressBar(NULL);
  m_AllocationStatus -> setMinimumWidth(100);
  m_AllocationStatus -> setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_AllocationStatus -> setFormat("%v/%m");
  m_AllocationStatus -> setTextVisible(true);
  m_AllocationStatus -> setToolTip(tr("Memory usage"));

  statusBar() -> addPermanentWidget(m_AllocationStatus);

  m_FileMenuP = file;
  m_EditMenuP = edit;
  m_WindowMenuP = window;

  QxrdApplicationPtr app(m_Application);
  QxrdExperimentPtr exper(m_Experiment);

  if (app) {
    QxrdApplicationSettingsPtr appset(app->settings());

    if (appset) {
      connect(&m_StatusTimer, &QTimer::timeout, this, &QxrdMainWindow::clearStatusMessage);
      connect(&m_UpdateTimer, &QTimer::timeout, this, &QxrdMainWindow::doTimerUpdate);

      m_UpdateTimer.start(appset->get_UpdateIntervalMsec());

      connect(appset->prop_UpdateIntervalMsec(), &QcepIntProperty::valueChanged, this, &QxrdMainWindow::onUpdateIntervalMsecChanged);
    }
  }

  updateTitle();

  setWindowIcon(QIcon(":/images/qxrd-icon-64x64.png"));

  if (m_FileMenuP && app && exper) {
    m_FileMenuP->clear();
    m_ActionNewExperiment =
        m_FileMenuP->addAction(tr("New Experiment..."), app.data(), &QxrdApplication::createNewExperiment);
//    QMenu *m_RecentExperiments = new QMenu();

    m_ActionOpenExperiment =
        m_FileMenuP->addAction(tr("Open Experiment..."), app.data(), &QxrdApplication::chooseExistingExperiment);

    m_ActionRecentExperiments =
        m_FileMenuP->addAction(tr("Recent Experiments"));

    m_RecentExperimentsMenu = new QMenu(this);

    m_ActionRecentExperiments -> setMenu(m_RecentExperimentsMenu);

    connect(m_RecentExperimentsMenu, &QMenu::aboutToShow,
            this,                    &QxrdMainWindow::populateRecentExperimentsMenu);

    m_ActionSaveExperiment =
        m_FileMenuP->addAction(tr("Save Experiment..."), exper.data(), &QxrdExperiment::saveExperiment);

    m_ActionSaveExperimentAs =
        m_FileMenuP->addAction(tr("Save Experiment As..."), this, &QxrdMainWindow::saveExperimentAs);

    m_ActionSaveExperimentCopy =
        m_FileMenuP->addAction(tr("Save Experiment Copy..."), this, &QxrdMainWindow::saveExperimentCopy);

    m_FileMenuP->addSeparator();

    m_ActionGlobalPreferences =
        m_FileMenuP->addAction(tr("Global Preferences..."), app.data(), &QxrdApplication::editGlobalPreferences);

    m_ActionExperimentPreferences =
        m_FileMenuP->addAction(tr("Experiment Preferences..."), this, &QxrdMainWindow::doEditPreferences);

    m_ActionQuit =
        m_FileMenuP->addAction(tr("Quit"), app.data(), &QxrdApplication::possiblyQuit);
  }

  if (m_EditMenuP) {
    connect(m_EditMenuP, &QMenu::aboutToShow, this, &QxrdMainWindow::populateEditMenu);

    m_ActionUndo = new QAction(this);
    m_ActionUndo->setObjectName(QStringLiteral("m_ActionUndo"));
    m_ActionUndo->setText(QApplication::translate("QxrdWindow", "Undo", Q_NULLPTR));

    m_ActionRedo = new QAction(this);
    m_ActionRedo->setObjectName(QStringLiteral("m_ActionRedo"));
    m_ActionRedo->setText(QApplication::translate("QxrdWindow", "Redo", Q_NULLPTR));

    m_ActionCut = new QAction(this);
    m_ActionCut->setObjectName(QStringLiteral("m_ActionCut"));
    m_ActionCut->setText(QApplication::translate("QxrdWindow", "Cut", Q_NULLPTR));

    m_ActionCopy = new QAction(this);
    m_ActionCopy->setObjectName(QStringLiteral("m_ActionCopy"));
    m_ActionCopy->setText(QApplication::translate("QxrdWindow", "Copy", Q_NULLPTR));

    m_ActionPaste = new QAction(this);
    m_ActionPaste->setObjectName(QStringLiteral("m_ActionPaste"));
    m_ActionPaste->setText(QApplication::translate("QxrdWindow", "Paste", Q_NULLPTR));

    m_ActionDelete = new QAction(this);
    m_ActionDelete->setObjectName(QStringLiteral("m_ActionDelete"));
    m_ActionDelete->setText(QApplication::translate("QxrdWindow", "Delete", Q_NULLPTR));

    m_ActionSelectAll = new QAction(this);
    m_ActionSelectAll->setObjectName(QStringLiteral("m_ActionSelectAll"));
    m_ActionSelectAll->setText(QApplication::translate("QxrdWindow", "Select All", Q_NULLPTR));

    connect(m_ActionUndo, &QAction::triggered, this, &QxrdMainWindow::doUndo);
    connect(m_ActionRedo, &QAction::triggered, this, &QxrdMainWindow::doRedo);
    connect(m_ActionCut, &QAction::triggered, this, &QxrdMainWindow::doCut);
    connect(m_ActionCopy, &QAction::triggered, this, &QxrdMainWindow::doCopy);
    connect(m_ActionPaste, &QAction::triggered, this, &QxrdMainWindow::doPaste);
    connect(m_ActionDelete, &QAction::triggered, this, &QxrdMainWindow::doDelete);
    connect(m_ActionSelectAll, &QAction::triggered, this, &QxrdMainWindow::doSelectAll);
  }

  if (m_WindowMenuP) {
    connect(m_WindowMenuP,  &QMenu::aboutToShow,
            this, &QxrdMainWindow::populateWindowsMenu);

  }

  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    connect(acq.data(), &QxrdAcquisition::acquireStarted,
            this,       &QxrdMainWindow::acquireStarted);

    connect(acq.data(), &QxrdAcquisition::acquiredFrame,
            this,       &QxrdMainWindow::acquiredFrame);

    connect(acq.data(), &QxrdAcquisition::acquireComplete,
            this,       &QxrdMainWindow::acquireComplete);
  }
}

void QxrdMainWindow::updateTitle()
{
  QxrdExperimentPtr exper(m_Experiment);

  QString title;

  title = m_Name;

  if (exper) {
    title.append(" - ");
    title.append(exper->experimentFilePath());

    exper -> prop_CompletionPercentage() -> linkTo(m_Progress);
  }

  title.append(" - QXRD");

  if (sizeof(void*) == 4) {
    title.append(" - 32 bit - v");
  } else {
    title.append(" - 64 bit - v");
  }

  title.append(STR(QXRD_VERSION));

  if (exper && exper->isChanged()) {
    title.append(tr(" [%1]").arg(exper->isChanged()));
  }

  setWindowTitle(title);
}

void QxrdMainWindow::populateEditMenu()
{
  QcepExperimentPtr expt(m_Experiment);

  m_EditMenuP -> clear();

  if (expt) {
    QAction *undoAction = NULL, *redoAction = NULL;

    QWidget* focusWidget = QApplication::focusWidget();

    if (focusWidget) {
//      printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

      QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
      QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
      QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
      QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
      QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);

      if (txed || lned || ispn || dspn || cbox) {
        undoAction = m_ActionUndo;
        redoAction = m_ActionRedo;
      }
    }

    if (undoAction == NULL) {
      undoAction = expt->undoStack()->createUndoAction(this);
    }

    if (redoAction == NULL) {
      redoAction = expt->undoStack()->createRedoAction(this);
    }

    m_EditMenuP->addAction(undoAction);
    m_EditMenuP->addAction(redoAction);
    m_EditMenuP->addSeparator();
    m_EditMenuP->addAction(m_ActionCut);
    m_EditMenuP->addAction(m_ActionCopy);
    m_EditMenuP->addAction(m_ActionPaste);
    m_EditMenuP->addAction(m_ActionDelete);
    m_EditMenuP->addSeparator();
    m_EditMenuP->addAction(m_ActionSelectAll);
  }
}

void QxrdMainWindow::populateWindowsMenu()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && m_WindowMenuP) {
    m_WindowMenuP->clear();

    for (int i=0; i<expt->windowSettingsCount(); i++) {
      QxrdMainWindowSettingsPtr set =
          qSharedPointerDynamicCast<QxrdMainWindowSettings>(expt->windowSettings(i));

      if (set) {
        QxrdMainWindowPtr win = set->window();
        QAction *act = NULL;

        if (win) {
          act = m_WindowMenuP ->
              addAction(tr("Show %1 Window").arg(set->get_Name()), this, &QxrdMainWindow::newWindow);
        } else {
          act = m_WindowMenuP ->
              addAction(tr("New %1 Window").arg(set->get_Name()), this, &QxrdMainWindow::newWindow);
        }

        if (act) {
          act->setData(i);
        }
      }
    }
  }
}

void QxrdMainWindow::populateRecentExperimentsMenu()
{
  //  printMessage("Populating recent experiments menu");

  m_RecentExperimentsMenu->clear();

  QxrdApplicationPtr app(m_Application);

  if (app) {
    QxrdApplicationSettingsPtr set(app->settings());

    if (set) {
      QStringList recent = set->get_RecentExperiments();

      foreach (QString exp, recent) {
        QAction *action = new QAction(exp, m_RecentExperimentsMenu);

        connect(action, &QAction::triggered, [=] {app->openRecentExperiment(exp);});

        m_RecentExperimentsMenu -> addAction(action);
      }
    }
  }
}

void QxrdMainWindow::newWindow()
{
  QxrdExperimentPtr expt(m_Experiment);

  QAction *act =
      qobject_cast<QAction*>(sender());

  if (act && expt) {
    int i = act->data().toInt();

    QxrdMainWindowSettingsPtr set =
        qSharedPointerDynamicCast<QxrdMainWindowSettings>(expt->windowSettings(i));

    if (set) {
      QxrdMainWindowPtr win = set->window();

      if (!win) {
        win =set->newWindow();
      }

      if (win) {
        win->show();
        win->raise();
        win->activateWindow();
      }
    }
  }
}

void QxrdMainWindow::printLine(QString /*line*/)
{
}

void QxrdMainWindow::printMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdMainWindow::criticalMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdMainWindow::statusMessage(QString /*msg*/, QDateTime /*ts*/)
{
}

void QxrdMainWindow::allocatedMemoryChanged()
{
  int alloc = QcepAllocator::allocatedMemoryMB();
  int avail = QcepAllocator::availableMemoryMB();

  m_AllocationStatus -> setMaximum(avail);
  m_AllocationStatus -> setValue(alloc);
}

void QxrdMainWindow::doTimerUpdate()
{
  updateTitle();

  allocatedMemoryChanged();
}

void QxrdMainWindow::displayStatusMessage(QString msg)
{
  if (QThread::currentThread()==thread()) {
    m_StatusMsg -> setText(msg);

    //  printMessage(msg);

    m_StatusTimer.start(5000);
  } else {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "displayStatusMessage",
                                           Qt::QueuedConnection,
                                           Q_ARG(QString, msg)));
  }
}

void QxrdMainWindow::onUpdateIntervalMsecChanged(int newVal)
{
  m_UpdateTimer.setInterval(newVal);
}

void QxrdMainWindow::clearStatusMessage()
{
  m_StatusMsg -> setText("");
}

void QxrdMainWindow::acquireStarted()
{
}

void QxrdMainWindow::acquireComplete()
{
  THREAD_CHECK;

  m_Progress -> reset();
}

void QxrdMainWindow::acquiredFrame(
    QString fileName, int iphase, int nphases, int isum, int nsum, int igroup, int ngroup)
{
  int totalFrames = (nphases*nsum*ngroup <= 0 ? 1 : nphases*nsum*ngroup);
  int thisFrame   = igroup*nphases*nsum + isum*nphases + iphase + 1;

  if (nphases <= 1) {
    displayStatusMessage(tr("%1: Exposure %2 of %3, File %4 of %5")
                         .arg(fileName)
                         .arg(isum+1).arg(nsum)
                         .arg(igroup+1).arg(ngroup));
  } else {
    displayStatusMessage(tr("%1: Phase %2 of %3, Sum %4 of %5, Group %6 of %7")
                         .arg(fileName)
                         .arg(iphase+1).arg(nphases)
                         .arg(isum+1).arg(nsum)
                         .arg(igroup+1).arg(ngroup));
  }

  m_Progress -> setValue(thisFrame*100/totalFrames);
}

void QxrdMainWindow::doEditPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPreferencesDialog prefs(m_Experiment, NULL, 1);

  prefs.exec();
}

void QxrdMainWindow::doEditDetectorPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPreferencesDialog prefs(m_Experiment, NULL, 0);

  prefs.exec();
}

void QxrdMainWindow::saveExperimentAs()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(m_Experiment);
  QxrdApplicationPtr app(m_Application);

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+"-copy.qxrdp");

    QString newChoice = QFileDialog::getSaveFileName(NULL,
                                                     "Save Experiment As",
                                                     newPath,
                                                     "QXRD Experiments (*.qxrdp)");

    if (newChoice.length()>0) {
      expt->saveExperimentAs(newChoice);
      app->appendRecentExperiment(newChoice);
    }
  }
}

void QxrdMainWindow::saveExperimentCopy()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPtr expt(m_Experiment);
  QxrdApplicationPtr app(m_Application);

  if (app && expt) {
    QString path = expt->experimentFilePath();
    QString name = expt->defaultExperimentName(path);
    QString dirp = expt->defaultExperimentDirectory(path);

    QDir dir(expt->get_ExperimentDirectory());

    QString newPath = dir.filePath(name+"-copy.qxrdp");

    QString newChoice = QFileDialog::getSaveFileName(NULL,
                                                     "Save Experiment Copy",
                                                     newPath,
                                                     "QXRD Experiments (*.qxrdp)");

    if (newChoice.length()>0) {
      expt->saveExperimentCopyAs(newChoice);
      app->appendRecentExperiment(newChoice);
    }
  }
}

void QxrdMainWindow::doUndo()
{
  QWidget* focusWidget = QApplication::focusWidget();

  if (focusWidget) {
//    printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

    QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
    QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
    QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
    QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
    QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);

    if (txed) {
      txed->undo();
    } else if (lned) {
      lned->undo();
    } else if (ispn) {
    } else if (dspn) {
    } else if (cbox) {
    }
  }
}

void QxrdMainWindow::doRedo()
{
  QWidget* focusWidget = QApplication::focusWidget();

  if (focusWidget) {
//    printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

    QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
    QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
    QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
    QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
    QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);

    if (txed) {
      txed->redo();
    } else if (lned) {
      lned->redo();
    } else if (ispn) {
    } else if (dspn) {
    } else if (cbox) {
    }
  }
}

void QxrdMainWindow::doCut()
{
  QWidget* focusWidget = QApplication::focusWidget();

  if (focusWidget) {
//    printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

    QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
    QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
    QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
    QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
    QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);

    if (txed) {
      txed->cut();
    } else if (lned) {
      lned->cut();
    } else if (ispn) {
    } else if (dspn) {
    } else if (cbox) {
    }
  }
}

void QxrdMainWindow::doCopy()
{
  QWidget* focusWidget = QApplication::focusWidget();

  if (focusWidget) {
//    printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

    QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
    QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
    QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
    QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
    QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);

    if (txed) {
      txed->copy();
    } else if (lned) {
      lned->copy();
    } else if (ispn) {
    } else if (dspn) {
    } else if (cbox) {
    }
  }
}

void QxrdMainWindow::doPaste()
{
  QWidget* focusWidget = QApplication::focusWidget();

  if (focusWidget) {
//    printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

    QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
    QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
    QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
    QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
    QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);

    if (txed) {
      txed->paste();
    } else if (lned) {
      lned->paste();
    } else if (ispn) {
    } else if (dspn) {
    } else if (cbox) {
    }
  }
}

void QxrdMainWindow::doDelete()
{
  QWidget* focusWidget = QApplication::focusWidget();

  if (focusWidget) {
//    printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

    QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
    QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
    QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
    QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
    QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);

    if (txed) {
      if (txed->textCursor().hasSelection()) {
        txed->textCursor().deleteChar();
      }
    } else if (lned) {
      if (lned->selectedText().length()) {
        lned->del();
      }
    } else if (ispn) {
    } else if (dspn) {
    } else if (cbox) {
    }
  }
}

void QxrdMainWindow::doSelectAll()
{
  QWidget* focusWidget = QApplication::focusWidget();

  if (focusWidget) {
//    printMessage(tr("focusWidget = %1").arg(focusWidget->objectName()));

    QTextEdit *txed = qobject_cast<QTextEdit*>(focusWidget);
    QLineEdit *lned = qobject_cast<QLineEdit*>(focusWidget);
    QSpinBox  *ispn = qobject_cast<QSpinBox*>(focusWidget);
    QDoubleSpinBox *dspn = qobject_cast<QDoubleSpinBox*>(focusWidget);
    QComboBox *cbox = qobject_cast<QComboBox*>(focusWidget);
    QAbstractItemView *itmv = qobject_cast<QAbstractItemView*>(focusWidget);

    if (txed) {
      txed->selectAll();
    } else if (lned) {
      lned->selectAll();
    } else if (ispn) {
    } else if (dspn) {
    } else if (cbox) {
    } else if (itmv) {
      itmv->selectAll();
    }
  }
}

