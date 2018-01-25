#include "qxrdwatcher.h"

QxrdWatcher::QxrdWatcher(QString name)
  : QcepObject(name),
    m_FilePattern(this,"filePattern","", "File Name Pattern")
{
#ifndef QT_NO_DEBUG
  printf("Constructing watcher\n");
#endif
}

QxrdWatcher::~QxrdWatcher()
{
#ifndef QT_NO_DEBUG
  printf("Deleting watcher\n");
#endif
}
