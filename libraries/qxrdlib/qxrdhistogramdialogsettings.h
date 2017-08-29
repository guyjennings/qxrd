#ifndef QXRDHISTOGRAMDIALOGSETTINGS_H
#define QXRDHISTOGRAMDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include <QObject>
#include "qcepproperty.h"
#include "qcepserializableobject.h"
#include "qxrdhistogramplotsettings-ptr.h"
#include "qxrdhistogramdialogsettings-ptr.h"

class QXRD_EXPORT QxrdHistogramDialogSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdHistogramDialogSettings(QString name);
  static QxrdHistogramDialogSettingsPtr newHistogramDialogSettings();

public:
  virtual void addChildPtr(QcepSerializableObjectPtr child);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdHistogramPlotSettingsWPtr histogramPlotSettings();

public:
  Q_PROPERTY(QRectF histogramRect READ get_HistogramRect WRITE set_HistogramRect)
  QCEP_DOUBLE_RECT_PROPERTY(HistogramRect)

private:
  QxrdHistogramPlotSettingsPtr  m_HistogramPlotSettings;
};

Q_DECLARE_METATYPE(QxrdHistogramDialogSettings*)

#endif // QXRDHISTOGRAMDIALOGSETTINGS_H
