#include "qxrddetectorsettingsepicsarea.h"

QxrdDetectorSettingsEpicsArea::QxrdDetectorSettingsEpicsArea(QString name) :
  QxrdDetectorSettings(name, EpicsAreaDetector)
{
}

QxrdDetectorSettingsEpicsArea::~QxrdDetectorSettingsEpicsArea()
{
}

void QxrdDetectorSettingsEpicsArea::pushDefaultsToProxy(QxrdDetectorProxyPtr /*proxy*/)
{
}
