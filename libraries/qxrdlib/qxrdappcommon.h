#ifndef QXRDAPPCOMMON_H
#define QXRDAPPCOMMON_H

#include "qxrdlib_global.h"
#include "qcepproperty.h"
#include "qcepapplication.h"

class QXRD_EXPORT QxrdAppCommon : public QcepApplication
{
  Q_OBJECT

public:
  QxrdAppCommon(int &argc, char **argv);
  virtual ~QxrdAppCommon();

  virtual bool init(int &argc, char **argv);
};

#endif // QXRDAPPCOMMON_H
