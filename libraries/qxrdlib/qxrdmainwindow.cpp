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
