#ifndef QXRDSLICEDIALOGSETTINGS_H
#define QXRDSLICEDIALOGSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdsliceplotsettings.h"
#include "qcepsettingssaver-ptr.h"

class QxrdSliceDialogSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdSliceDialogSettings(QcepSettingsSaverWPtr saver, QcepObject *parent);
  
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
  QxrdSlicePlotSettingsPtr m_SlicePlotSettings;
};

typedef QSharedPointer<QxrdSliceDialogSettings> QxrdSliceDialogSettingsPtr;
typedef QWeakPointer<QxrdSliceDialogSettings>   QxrdSliceDialogSettingsWPtr;

#endif // QXRDSLICEDIALOGSETTINGS_H
