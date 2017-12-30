#ifndef QXRDEXTRAINPUTSPLOTWIDGETSETTINGS_H
#define QXRDEXTRAINPUTSPLOTWIDGETSETTINGS_H

#include "qxrdplotwidgetsettings.h"

class QXRD_EXPORT QxrdExtraInputsPlotWidgetSettings : public QxrdPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdExtraInputsPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdExtraInputsPlotWidgetSettings*)

#endif // QXRDEXTRAINPUTSPLOTWIDGETSETTINGS_H
