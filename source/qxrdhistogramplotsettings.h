#ifndef QXRDHISTOGRAMPLOTSETTINGS_H
#define QXRDHISTOGRAMPLOTSETTINGS_H

#include "qxrdplotsettings.h"

class QxrdHistogramPlotSettings : public QxrdPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdHistogramPlotSettings(QxrdSettingsSaverWPtr saver, QObject *parent);
  
signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdHistogramPlotSettings> QxrdHistogramPlotSettingsPtr;
typedef QWeakPointer<QxrdHistogramPlotSettings>   QxrdHistogramPlotSettingsWPtr;

#endif // QXRDHISTOGRAMPLOTSETTINGS_H
