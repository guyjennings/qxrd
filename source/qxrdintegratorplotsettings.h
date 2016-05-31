#ifndef QXRDINTEGRATORPLOTSETTINGS_H
#define QXRDINTEGRATORPLOTSETTINGS_H

#include <QObject>
#include "qcepplotsettings.h"
#include "qxrdintegratorplotsettings-ptr.h"

class QxrdIntegratorPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdIntegratorPlotSettings(QString name);
  static QxrdIntegratorPlotSettingsPtr newIntegratorPlotSettings();
};

Q_DECLARE_METATYPE(QxrdIntegratorPlotSettings*)

#endif // QXRDINTEGRATORPLOTSETTINGS_H
