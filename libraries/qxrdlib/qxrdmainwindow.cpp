#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrdmainwindow.h"
#include <QMenu>
#include <QMenuBar>

QxrdMainWindow::QxrdMainWindow(QString name, QxrdApplicationPtr app, QxrdExperimentPtr expt)
  : QcepMainWindow(),
    m_Application(app),
    m_Experiment(expt),
    m_MainWindowsMenu(NULL)
{

}

void QxrdMainWindow::setupMenus()
{
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
  if (m_MainWindowsMenu) {
    m_MainWindowsMenu->clear();
    m_MainWindowsMenu->addAction("New Acquisition Window"/*,
                             &QxrdAcquisitionWindow::newWindow*/);
  }
}
