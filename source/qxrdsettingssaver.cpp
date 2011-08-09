#include "qxrdsettingssaver.h"
#include "qcepproperty.h"
#include "qxrdapplication.h"
#include <stdio.h>

QxrdSettingsSaver::QxrdSettingsSaver(QObject *parent, QxrdApplication *app) :
    QObject(parent),
    m_Application(app),
    m_SaveDelay(5000)
{
  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(performSave()));

  m_Timer.setSingleShot(false);

  m_Timer.start(m_SaveDelay);
}

void QxrdSettingsSaver::performSave()
{
  int nupdates = QcepProperty::getChangeCount();

  if (nupdates > 0) {
    if (qcepDebug(DEBUG_PREFS)) {
      g_Application->printMessage(tr("Settings Saver saving %1 updates").arg(nupdates));
    }

    QTime tic;
    tic.start();

    INVOKE_CHECK(QMetaObject::invokeMethod(m_Application, "writeSettings", Qt::BlockingQueuedConnection));

    if (qcepDebug(DEBUG_PREFS)) {
      g_Application->printMessage(tr("Saving settings took %1 msec").arg(tic.elapsed()));
    }
  }
}
