#include "qxrdsimulatedsettings.h"
#include "qxrdsimulatedsettings-ptr.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qcepallocator.h"
#include "qxrddebug.h"
#include "qxrdsimulateddialog.h"
#include "qxrdsimulateddialog-ptr.h"
#include <stdio.h>
#include <QPainter>

QxrdSimulatedSettings::QxrdSimulatedSettings(QString name) :
  QxrdDetectorSettings(name, Simulated)
{
#ifndef QT_NO_DEBUG
  printf("Constructing simulated detector\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSimulated::QxrdDetectorSimulated(%p)\n", this);
  }
}

QxrdSimulatedSettings::~QxrdSimulatedSettings()
{
#ifndef QT_NO_DEBUG
  printf("Deleting simulated detector\n");
#endif

  if (QThread::currentThread() != thread()) {
    printf("Deleting simulated detector from wrong thread\n");
  }

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDetectorSimulated::~QxrdDetectorSimulated(%p)\n", this);
  }
}

void QxrdSimulatedSettings::configureDetector()
{
  GUI_THREAD_CHECK;

  QxrdSimulatedSettingsPtr myself =
      qSharedPointerDynamicCast<QxrdSimulatedSettings>(sharedFromThis());

  QxrdSimulatedDialogPtr dlog =
      QxrdSimulatedDialogPtr(
        new QxrdSimulatedDialog(myself));

  if (dlog) {
    dlog->exec();
  }
}
