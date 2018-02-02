#ifndef QCEPAPPLICATIONSETTINGS_H
#define QCEPAPPLICATIONSETTINGS_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include "qcepapplication-ptr.h"

class QCEP_EXPORT QcepApplicationSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepApplicationSettings(QString name);
  void initialize(QcepObjectWPtr parent);
};

extern QCEP_EXPORT QcepApplicationSettings *g_ApplicationSettings;

#endif // QCEPAPPLICATIONSETTINGS_H
