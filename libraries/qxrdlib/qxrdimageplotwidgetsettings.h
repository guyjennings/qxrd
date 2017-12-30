#ifndef QXRDIMAGEPLOTWIDGETSETTINGS_H
#define QXRDIMAGEPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdImagePlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdImagePlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdImagePlotWidgetSettings*)

#endif // QXRDIMAGEPLOTWIDGETSETTINGS_H
