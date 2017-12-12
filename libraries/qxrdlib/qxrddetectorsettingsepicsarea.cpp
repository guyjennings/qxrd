#include "qxrddetectorsettingsepicsarea.h"
#include "qxrdareadetectordialog.h"
#include "qxrdareadetectordialog-ptr.h"

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

void QxrdDetectorSettingsEpicsArea::configureDetector()
{
  QxrdAreaDetectorDialogPtr dlg =
      QxrdAreaDetectorDialogPtr(
        new QxrdAreaDetectorDialog());

  if (dlg) {
    dlg->exec();
  }
}
