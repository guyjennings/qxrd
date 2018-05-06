#ifndef QSWGLOBALSETTINGS_H
#define QSWGLOBALSETTINGS_H

#include "qswlib_global.h"
#include "qcepmacros.h"
#include <QSettings>
#include "qswapplication-ptr.h"

class /*QSW_EXPORT*/ QswGlobalSettings : public QSettings
{
  Q_OBJECT

private:
  typedef QSettings inherited;

public:
  QswGlobalSettings();
};

#endif // QSWGLOBALSETTINGS_H
