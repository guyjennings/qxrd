#ifndef QXRDHISTOGRAMDIALOGSETTINGS_H
#define QXRDHISTOGRAMDIALOGSETTINGS_H

#include <QObject>
#include "qcepproperty.h"
#include "qcepobject.h"
#include "qxrdhistogramplotsettings-ptr.h"

class QxrdHistogramDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdHistogramDialogSettings(QString name);
  QxrdHistogramDialogSettings();

public:
  virtual void addChildPtr(QcepObjectPtr child);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdHistogramPlotSettingsWPtr histogramPlotSettings();

public:
  Q_PROPERTY(QRectF histogramRect READ get_HistogramRect WRITE set_HistogramRect)
  QCEP_DOUBLE_RECT_PROPERTY(HistogramRect)

private:
  QxrdHistogramPlotSettingsPtr  m_HistogramPlotSettings;
};

Q_DECLARE_METATYPE(QxrdHistogramDialogSettings*)

#endif // QXRDHISTOGRAMDIALOGSETTINGS_H
