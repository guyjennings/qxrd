#include "qxrdareadetectorsettings.h"
#include "qxrdareadetectorsettings-ptr.h"
#include "qxrdareadetectordialog.h"
#include "qxrdareadetectordialog-ptr.h"
#include <QThread>

QxrdAreaDetectorSettings::QxrdAreaDetectorSettings(QString name) :
  QxrdDetectorSettings(name, AreaDetector)
{
}

QxrdAreaDetectorSettings::~QxrdAreaDetectorSettings()
{
}

void QxrdAreaDetectorSettings::configureDetector()
{
  GUI_THREAD_CHECK;

  QxrdAreaDetectorSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdAreaDetectorSettings>(sharedFromThis());

  QxrdAreaDetectorDialogPtr dlg =
      QxrdAreaDetectorDialogPtr(
        new QxrdAreaDetectorDialog(myself));

  if (dlg) {
    dlg->exec();
  }
}
