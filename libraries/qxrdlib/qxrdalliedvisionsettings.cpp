#include "qxrdalliedvisionsettings.h"
#include "qxrdalliedvisionsettings-ptr.h"
#include "qxrdexperiment.h"
#include "qxrdacqcommon.h"
#include "qcepallocator.h"
#include "qxrddebug.h"
#include "qxrdalliedvisiondialog.h"
#include "qxrdalliedvisiondialog-ptr.h"
#include <stdio.h>
#include <QPainter>
#include <QThread>

QxrdAlliedVisionSettings::QxrdAlliedVisionSettings(QString name) :
  QxrdDetectorSettings(name, AlliedVision)
{
#ifndef QT_NO_DEBUG
  printf("Constructing simulated detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAlliedVisionSettings::QxrdAlliedVisionSettings(%p)\n", this);
  }
}

QxrdAlliedVisionSettings::~QxrdAlliedVisionSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting Allied Vision Settings\n");
#endif

  if (QThread::currentThread() != thread()) {
    printf("Deleting Allied Vision Settings from wrong thread\n");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAlliedVisionSettings::~QxrdAlliedVisionSettings(%p)\n", this);
  }
}

void QxrdAlliedVisionSettings::configureDetector()
{
  GUI_THREAD_CHECK;

  QxrdAlliedVisionSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdAlliedVisionSettings>(sharedFromThis());

  QxrdAlliedVisionDialogPtr dlog =
      QxrdAlliedVisionDialogPtr(
        NEWPTR(QxrdAlliedVisionDialog(myself)));

  if (dlog) {
    dlog->exec();
  }
}
