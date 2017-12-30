#ifndef QXRDPLOTWIDGETSETTINGS_H
#define QXRDPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"

class QXRD_EXPORT QxrdPlotWidgetSettings : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdPlotWidgetSettings*)

#endif // QXRDPLOTWIDGETSETTINGS_H
