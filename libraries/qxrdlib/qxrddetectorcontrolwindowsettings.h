#ifndef QXRDDETECTORCONTROLWINDOWSETTINGS_H
#define QXRDDETECTORCONTROLWINDOWSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrddetectorcontrolwindowsettings-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdDetectorControlWindowSettings : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdDetectorControlWindowSettings(QString name);
  void initialize(QcepObjectWPtr parent);

  void writeSettings(QSettings *set);
  void readSettings(QSettings *set);

  static void registerMetaTypes();

  QcepImagePlotWidgetSettingsPtr imagePlotWidgetSettings();

private:
  QxrdDetectorSettingsWPtr       m_DetectorSettings;
  QcepImagePlotWidgetSettingsPtr m_ImagePlotWidgetSettings;

public:
  Q_PROPERTY(int detectorWindowOpen READ get_DetectorWindowOpen WRITE set_DetectorWindowOpen)
  QCEP_INTEGER_PROPERTY(DetectorWindowOpen)

  Q_PROPERTY(QByteArray detectorWindowGeometry READ get_DetectorWindowGeometry WRITE set_DetectorWindowGeometry)
  QCEP_BYTE_ARRAY_PROPERTY(DetectorWindowGeometry)

  Q_PROPERTY(QByteArray detectorWindowState READ get_DetectorWindowState WRITE set_DetectorWindowState)
  QCEP_BYTE_ARRAY_PROPERTY(DetectorWindowState)

  Q_PROPERTY(int detectorWindowNewROIType READ get_DetectorWindowNewROIType WRITE set_DetectorWindowNewROIType)
  QCEP_INTEGER_PROPERTY(DetectorWindowNewROIType)

  Q_PROPERTY(QRectF detectorWindowRect READ get_DetectorWindowRect WRITE set_DetectorWindowRect)
  QCEP_DOUBLE_RECT_PROPERTY(DetectorWindowRect)
};

#endif // QXRDDETECTORCONTROLWINDOWSETTINGS_H
