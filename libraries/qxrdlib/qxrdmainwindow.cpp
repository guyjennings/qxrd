#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrdmainwindow.h"
#include "qxrdexperiment.h"
#include "qxrdmainwindowsettings-ptr.h"
#include "qxrdmainwindowsettings.h"
#include <QMenu>
#include <QMenuBar>

QxrdMainWindow::QxrdMainWindow(QString name, QxrdApplicationWPtr app, QxrdExperimentWPtr expt)
  : QcepMainWindow(),
    m_Name(name),
    m_Application(app),
    m_Experiment(expt),
    m_MainWindowsMenu(NULL)
{
}

void QxrdMainWindow::setupMenus()
{
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

  QMenuBar *mbar = menuBar();

  if (mbar) {
    mbar->addSeparator();

    m_MainWindowsMenu = new QMenu("xxx");

    mbar->addMenu(m_MainWindowsMenu);

    connect(m_MainWindowsMenu,  &QMenu::aboutToShow,
            this, &QxrdMainWindow::populateWindowsMenu);
  }
}

void QxrdMainWindow::populateWindowsMenu()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt && m_MainWindowsMenu) {
    m_MainWindowsMenu->clear();

    for (int i=0; i<expt->windowSettingsCount(); i++) {
      QxrdMainWindowSettingsPtr set =
          qSharedPointerDynamicCast<QxrdMainWindowSettings>(expt->windowSettings(i));

      if (set) {
        QxrdMainWindowPtr win = set->window();
        QAction *act = NULL;

        if (win) {
          act = m_MainWindowsMenu ->
              addAction(tr("Show %1 Window").arg(set->get_Name()), this, &QxrdMainWindow::newWindow);
        } else {
          act = m_MainWindowsMenu ->
              addAction(tr("New %1 Window").arg(set->get_Name()), this, &QxrdMainWindow::newWindow);
        }

        if (act) {
          act->setData(i);
        }
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
