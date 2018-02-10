#include "qxrdapplicationsettings.h"
#include "qxrdapplication.h"
#include <QDir>

QxrdApplicationSettings::QxrdApplicationSettings(QString name,
                                                 int argc,
                                                 char **argv) :
  inherited(name, argc, argv),
  m_LockerCount(this, "lockerCount", 0, "Number of mutex locks taken"),
  m_LockerRate(this, "lockerRate", 0, "Mutex Locking Rate")
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

void QxrdApplicationSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
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
