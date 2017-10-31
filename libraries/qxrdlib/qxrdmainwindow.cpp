#include "qcepmacros.h"
#include "qxrddebug.h"
#include "qxrdmainwindow.h"
#include <QMenu>
#include <QMenuBar>

QxrdMainWindow::QxrdMainWindow(QWidget *parent) : QcepMainWindow(parent)
{

}

void QxrdMainWindow::setupMenus()
{
  QMenuBar *mbar = menuBar();

  if (mbar) {
    m_MainWindowsMenu = mbar->addMenu("Windows");

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
