#ifndef QXRDSLICEDIALOGSETTINGS_H
#define QXRDSLICEDIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdsliceplotsettings-ptr.h"
#include "qcepsettingssaver-ptr.h"

class QxrdSliceDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSliceDialogSettings(QString name);
  QxrdSliceDialogSettings();
  
  virtual void addChildPtr(QcepObjectPtr child);

public:
  Q_PROPERTY(QcepPolygon slicePolygon READ get_SlicePolygon WRITE set_SlicePolygon)
  QCEP_POLYGON_PROPERTY(SlicePolygon)

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  QxrdSlicePlotSettingsWPtr slicePlotSettings();

signals:
  
public slots:

private:
  QMutex                   m_Mutex;
  QxrdSlicePlotSettingsPtr m_SlicePlotSettings;
};

Q_DECLARE_METATYPE(QxrdSliceDialogSettings*)

#endif // QXRDSLICEDIALOGSETTINGS_H
