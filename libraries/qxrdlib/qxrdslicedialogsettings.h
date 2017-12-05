#ifndef QXRDSLICEDIALOGSETTINGS_H
#define QXRDSLICEDIALOGSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdsliceplotsettings-ptr.h"
#include "qxrdslicedialogsettings-ptr.h"

class QXRD_EXPORT QxrdSliceDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdSliceDialogSettings(QString name);
  static QxrdSliceDialogSettingsPtr newSliceDialogSettings();
  
  virtual void addChildPtr(QcepObjectPtr child);

public:
  Q_PROPERTY(QcepPolygon slicePolygon READ get_SlicePolygon WRITE set_SlicePolygon)
  QCEP_POLYGON_PROPERTY(SlicePolygon)

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  QxrdSlicePlotSettingsWPtr slicePlotSettings();

signals:
  
public slots:

private:
  QMutex                   m_Mutex;
  QxrdSlicePlotSettingsPtr m_SlicePlotSettings;
};

Q_DECLARE_METATYPE(QxrdSliceDialogSettings*)

#endif // QXRDSLICEDIALOGSETTINGS_H
