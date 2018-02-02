#ifndef QXRDAPPLICATIONSETTINGS_H
#define QXRDAPPLICATIONSETTINGS_H

#include "qxrdlib_global.h"
#include "qxrdappcommonsettings.h"
#include "qxrdapplication-ptr.h"

class QXRD_EXPORT QxrdApplicationSettings : public QxrdAppCommonSettings
{
  Q_OBJECT

private:
  typedef QxrdAppCommonSettings inherited;

public:
  QxrdApplicationSettings(QString name,
                          int argc,
                          char **argv);
  virtual ~QxrdApplicationSettings();
  void initialize(QcepObjectWPtr parent);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public:
//  Q_PROPERTY(bool   openDirectly  READ get_OpenDirectly WRITE set_OpenDirectly)
//  QCEP_BOOLEAN_PROPERTY(OpenDirectly)

  Q_PROPERTY(int lockerCount READ get_LockerCount WRITE set_LockerCount STORED false)
  QCEP_INTEGER_PROPERTY(LockerCount)

  Q_PROPERTY(double lockerRate READ get_LockerRate WRITE set_LockerRate STORED false)
  QCEP_DOUBLE_PROPERTY(LockerRate)
};

#endif // QXRDAPPLICATIONSETTINGS_H
