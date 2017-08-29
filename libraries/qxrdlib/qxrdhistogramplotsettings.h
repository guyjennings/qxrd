#ifndef QXRDHISTOGRAMPLOTSETTINGS_H
#define QXRDHISTOGRAMPLOTSETTINGS_H

#include "qcepplotsettings.h"
#include "qxrdhistogramplotsettings-ptr.h"

class QxrdHistogramPlotSettings : public QcepPlotSettings
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdHistogramPlotSettings(QString name);
  static QxrdHistogramPlotSettingsPtr newHistogramPlotSettings();

signals:
  
public slots:
  
};

#endif // QXRDHISTOGRAMPLOTSETTINGS_H
