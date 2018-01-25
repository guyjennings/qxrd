#ifndef QXRDWATCHER_H
#define QXRDWATCHER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"

class QXRD_EXPORT QxrdWatcher : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdWatcher(QString name);
  ~QxrdWatcher();

public:
  Q_PROPERTY(QString filePattern     READ get_FilePattern WRITE set_FilePattern)
  QCEP_STRING_PROPERTY(FilePattern)
};

#endif // QXRDWATCHER_H
