#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrdmainwindow.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdmainwindowsettings.h"
#include "qxrdexperimentpreferencesdialog.h"
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QFileDialog>
#include <QSignalMapper>
#include "qxrdapplicationsettings.h"

QxrdMainWindow::QxrdMainWindow(QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt)
  : QcepMainWindow(),
    m_Name(name),
    m_Application(app),
    m_Experiment(expt),
    m_FileMenuP(NULL),
    m_EditMenuP(NULL),
    m_WindowMenuP(NULL)
{
}

void QxrdMainWindow::setupMenus(QMenu *file, QMenu *edit, QMenu *window)
{
  m_FileMenuP = file;
  m_EditMenuP = edit;
  m_WindowMenuP = window;

  QxrdApplicationPtr app(m_Application);
  QxrdExperimentPtr exper(m_Experiment);
  QString title;

  title = m_Name;

  if (exper) {
    title.append(" - ");
    title.append(exper->experimentFilePath());
  }

  title.append(" - QXRD");

  if (sizeof(void*) == 4) {
    title.append(" - 32 bit - v");
  } else {
    title.append(" - 64 bit - v");
  }

  title.append(STR(QXRD_VERSION));

  setWindowTitle(title);

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

  if (m_WindowMenuP) {
    connect(m_WindowMenuP,  &QMenu::aboutToShow,
            this, &QxrdMainWindow::populateWindowsMenu);

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
        QSignalMapper *mapper = new QSignalMapper(action);
        connect(action, &QAction::triggered, mapper, (void (QSignalMapper::*)()) &QSignalMapper::map);
        mapper->setMapping(action, exp);

        connect(mapper, (void (QSignalMapper::*)(const QString&)) &QSignalMapper::mapped, app.data(),
                &QxrdApplication::openRecentExperiment);

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

void QxrdMainWindow::printLine(QString line)
{
}

void QxrdMainWindow::printMessage(QString msg, QDateTime ts)
{
}

void QxrdMainWindow::criticalMessage(QString msg, QDateTime ts)
{
}

void QxrdMainWindow::statusMessage(QString msg, QDateTime ts)
{
}

void QxrdMainWindow::doEditPreferences()
{
  GUI_THREAD_CHECK;

  QxrdExperimentPreferencesDialog prefs(m_Experiment, NULL, 1);

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

