#ifndef QXRDDETECTORCONTROLWINDOWSETTINGS_H
#define QXRDDETECTORCONTROLWINDOWSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qxrddetectorcontrolwindowsettings-ptr.h"
#include "qxrddetectorsettings-ptr.h"

class QxrdDetectorControlWindowSettings : public QcepSerializableObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDetectorControlWindowSettings(QxrdDetectorSettingsWPtr det);
  static QxrdDetectorControlWindowSettingsPtr newDetectorWindowSettings(QxrdDetectorSettingsWPtr det);

  virtual void addChildPtr(QcepSerializableObjectPtr child);

  static void registerMetaTypes();

private:
  QxrdDetectorSettingsWPtr m_DetectorSettings;

public:
  Q_PROPERTY(int detectorWindowOpen READ get_DetectorWindowOpen WRITE set_DetectorWindowOpen)
  QCEP_INTEGER_PROPERTY(DetectorWindowOpen)

  Q_PROPERTY(QByteArray detectorWindowGeometry READ get_DetectorWindowGeometry WRITE set_DetectorWindowGeometry)
  QCEP_BYTE_ARRAY_PROPERTY(DetectorWindowGeometry)

  Q_PROPERTY(QByteArray detectorWindowState READ get_DetectorWindowState WRITE set_DetectorWindowState)
  QCEP_BYTE_ARRAY_PROPERTY(DetectorWindowState)
};

#endif // QXRDDETECTORCONTROLWINDOWSETTINGS_H
