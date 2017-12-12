#include "qxrdareadetectorsettings.h"
#include "qxrdareadetectordialog.h"
#include "qxrdareadetectordialog-ptr.h"

QxrdAreaDetectorSettings::QxrdAreaDetectorSettings(QString name) :
  QxrdDetectorSettings(name, EpicsAreaDetector)
{
}

QxrdAreaDetectorSettings::~QxrdAreaDetectorSettings()
{
}

void QxrdAreaDetectorSettings::pushDefaultsToProxy(QxrdDetectorProxyPtr /*proxy*/)
{
}

void QxrdAreaDetectorSettings::configureDetector()
{
  QxrdAreaDetectorDialogPtr dlg =
      QxrdAreaDetectorDialogPtr(
        new QxrdAreaDetectorDialog());

  if (dlg) {
    dlg->exec();
  }
}
