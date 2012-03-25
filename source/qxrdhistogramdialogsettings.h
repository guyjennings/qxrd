#ifndef QXRDHISTOGRAMDIALOGSETTINGS_H
#define QXRDHISTOGRAMDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qxrdhistogramplotsettings.h"

class QxrdHistogramDialogSettings : public QObject
{
  Q_OBJECT
public:
  explicit QxrdHistogramDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent = 0);
  
public:
  Q_PROPERTY(QwtDoubleRect histogramRect READ get_HistogramRect WRITE set_HistogramRect)
  QCEP_DOUBLE_RECT_PROPERTY(HistogramRect)

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdHistogramPlotSettingsWPtr histogramPlotSettings();
signals:
  
public slots:
  
private:
  QxrdHistogramPlotSettingsPtr  m_HistogramPlotSettings;
};

typedef QSharedPointer<QxrdHistogramDialogSettings> QxrdHistogramDialogSettingsPtr;
typedef QWeakPointer<QxrdHistogramDialogSettings>   QxrdHistogramDialogSettingsWPtr;

#endif // QXRDHISTOGRAMDIALOGSETTINGS_H