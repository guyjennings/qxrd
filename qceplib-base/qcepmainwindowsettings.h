#ifndef QCEPMAINWINDOWSETTINGS_H
#define QCEPMAINWINDOWSETTINGS_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepserializableobject.h"

class QCEP_EXPORT QcepMainWindowSettings : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepMainWindowSettings(QString name);
};

Q_DECLARE_METATYPE(QcepMainWindowSettings*)

#endif // QCEPMAINWINDOWSETTINGS_H
