#ifndef QXRDACQDUMMY_H
#define QXRDACQDUMMY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qxrdacqcommon.h"

#include "qxrdacqdummy-ptr.h"

class QXRD_EXPORT QxrdAcqDummy : public QxrdAcqCommon
{
  Q_OBJECT

private:
  typedef QxrdAcqCommon inherited;

public:
  Q_INVOKABLE QxrdAcqDummy(QString name);
  ~QxrdAcqDummy();

  void initialize();

  static QxrdAcqDummyPtr newAcquisition();
};

Q_DECLARE_METATYPE(QxrdAcqDummy*)

#endif // QXRDACQDUMMY_H
