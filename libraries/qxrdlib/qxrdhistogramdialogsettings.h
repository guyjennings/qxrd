#ifndef QXRDHISTOGRAMDIALOGSETTINGS_H
#define QXRDHISTOGRAMDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include <QObject>
#include "qcepproperty.h"
#include "qcepobject.h"
#include "qxrdhistogramplotsettings-ptr.h"
#include "qxrdhistogramdialogsettings-ptr.h"
#include "qxrdhistogramplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdHistogramDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdHistogramDialogSettings(QString name);
  static QxrdHistogramDialogSettingsPtr newHistogramDialogSettings();

public:
  virtual void addChildPtr(QcepObjectPtr child);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdHistogramPlotSettingsWPtr histogramPlotSettings();
  QxrdHistogramPlotWidgetSettingsPtr histogramPlotWidgetSettings();

public:
  Q_PROPERTY(QRectF histogramRect READ get_HistogramRect WRITE set_HistogramRect)
  QCEP_DOUBLE_RECT_PROPERTY(HistogramRect)

private:
  QxrdHistogramPlotSettingsPtr       m_HistogramPlotSettings;
  QxrdHistogramPlotWidgetSettingsPtr m_HistogramPlotWidgetSettings;
};

Q_DECLARE_METATYPE(QxrdHistogramDialogSettings*)

#endif // QXRDHISTOGRAMDIALOGSETTINGS_H
