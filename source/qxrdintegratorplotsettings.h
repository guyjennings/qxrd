#ifndef QXRDINTEGRATORPLOTSETTINGS_H
#define QXRDINTEGRATORPLOTSETTINGS_H

#include <QObject>
#include "qxrdimageplotsettings.h"

class QxrdIntegratorPlotSettings : public QxrdImagePlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdIntegratorPlotSettings(QString name);
  QxrdIntegratorPlotSettings();
};

Q_DECLARE_METATYPE(QxrdIntegratorPlotSettings*)

#endif // QXRDINTEGRATORPLOTSETTINGS_H
