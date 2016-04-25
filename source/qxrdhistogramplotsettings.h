#ifndef QXRDHISTOGRAMPLOTSETTINGS_H
#define QXRDHISTOGRAMPLOTSETTINGS_H

#include "qcepplotsettings.h"

class QxrdHistogramPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  explicit QxrdHistogramPlotSettings(QcepSettingsSaverWPtr saver, QcepObjectWPtr parent);
  
signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdHistogramPlotSettings> QxrdHistogramPlotSettingsPtr;
typedef QWeakPointer<QxrdHistogramPlotSettings>   QxrdHistogramPlotSettingsWPtr;

#endif // QXRDHISTOGRAMPLOTSETTINGS_H
