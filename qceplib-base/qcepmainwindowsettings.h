#ifndef QCEPMAINWINDOWSETTINGS_H
#define QCEPMAINWINDOWSETTINGS_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"

class QCEP_EXPORT QcepMainWindowSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepMainWindowSettings(QString name);

  virtual void openWindow() = 0;
  virtual void closeWindow() = 0;
};

Q_DECLARE_METATYPE(QcepMainWindowSettings*)

#endif // QCEPMAINWINDOWSETTINGS_H
