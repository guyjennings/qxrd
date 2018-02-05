#ifndef QXRDACQCOMMON_H
#define QXRDACQCOMMON_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qxrdacqcommon-ptr.h"

class QXRD_EXPORT QxrdAcqCommon : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdAcqCommon(QString name);
  ~QxrdAcqCommon();

  virtual void initialize(QObjectWPtr);
};

#endif // QXRDACQCOMMON_H
