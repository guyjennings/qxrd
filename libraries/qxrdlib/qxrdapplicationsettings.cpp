#include "qxrdapplicationsettings.h"
#include "qxrdapplication.h"
#include <QDir>

QxrdApplicationSettings::QxrdApplicationSettings(QxrdApplicationWPtr app, int argc, char **argv) :
  inherited(app, argc, argv),
  m_CurrentDirectory(this, "currentDirectory", QDir::homePath(), "Current Directory"),
  //  m_OpenDirectly(m_Saver, this,"openDirectly", false, "Open Last Experiment at Startup"),
  m_FileBrowserLimit(this, "fileBrowserLimit", 1000, "Max Number of Files in Browser Windows (0 = unlimited)"),
  m_MessageWindowLines(this, "messageWindowLines", 1000, "Number of Lines in Message Window (0 = unlimited)"),
  m_UpdateIntervalMsec(this, "updateIntervalMsec", 1000, "Time Intervale for Updates (in msec)"),
  m_LockerCount(this, "lockerCount", 0, "Number of mutex locks taken"),
  m_LockerRate(this, "lockerRate", 0, "Mutex Locking Rate"),
  m_ExperimentCount(this, "experimentCount", 0, "Number of open experiments")
{
#ifndef QT_NO_DEBUG
  printf("Constructing application settings\n");
#endif
}

QxrdApplicationSettings::~QxrdApplicationSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting application settings\n");
#endif
}

void QxrdApplicationSettings::init()
{
  inherited::init();
}

void QxrdApplicationSettings::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

//  QcepAllocatorPtr alloc(m_Allocator);

//  if (alloc) {
//    settings->beginGroup("allocator");
//    alloc->readSettings(settings);
//    settings->endGroup();
//  }
}

void QxrdApplicationSettings::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

//  QcepAllocatorPtr alloc(m_Allocator);

//  if (alloc) {
//    settings->beginGroup("allocator");
//    alloc->writeSettings(settings);
//    settings->endGroup();
//  }
}
