#include "qxrddexelasettings.h"
#include "qxrddexelasettings-ptr.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrddebug.h"
#include "qxrddexeladialog.h"
#include "qxrddexeladialog-ptr.h"

#include <stdio.h>
#include <QPainter>
#include <QThread>

QxrdDexelaSettings::QxrdDexelaSettings(QString name) :
  QxrdDetectorSettings(name, Dexela),
  m_DetectorIndex  (this, "detectorIndex",   0, "Dexela Detector Number")
{
#ifndef QT_NO_DEBUG
  printf("Constructing dexela detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorDexela::QxrdDetectorDexela(%p)\n", this);
  }
}

QxrdDexelaSettings::~QxrdDexelaSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Dexela detector\n");
#endif

  if (QThread::currentThread() != thread()) {
    printf("Deleting Dexela detector from wrong thread\n");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorDexela::~QxrdDetectorDexela(%p)\n", this);
  }
}

void QxrdDexelaSettings::configureDetector()
{
  GUI_THREAD_CHECK;

  QxrdDexelaSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdDexelaSettings>(sharedFromThis());

  QxrdDexelaDialogPtr dlg =
      QxrdDexelaDialogPtr(
        new QxrdDexelaDialog(myself));

  if (dlg) {
    dlg->exec();
  }
}
