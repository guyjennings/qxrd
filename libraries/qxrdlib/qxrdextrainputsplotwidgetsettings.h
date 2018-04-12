#ifndef QXRDEXTRAINPUTSPLOTWIDGETSETTINGS_H
#define QXRDEXTRAINPUTSPLOTWIDGETSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepplotwidgetsettings.h"

class QXRD_EXPORT QxrdExtraInputsPlotWidgetSettings : public QcepPlotWidgetSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdExtraInputsPlotWidgetSettings(QString name);
};

Q_DECLARE_METATYPE(QxrdExtraInputsPlotWidgetSettings*)

#endif // QXRDEXTRAINPUTSPLOTWIDGETSETTINGS_H
