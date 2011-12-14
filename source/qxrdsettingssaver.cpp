#include "qxrdsettingssaver.h"
#include "qcepproperty.h"
#include "qxrdapplication.h"
#include <stdio.h>

QxrdSettingsSaver::QxrdSettingsSaver(QObject *parent, QObject *owner) :
  QObject(parent),
  m_Owner(owner),
  m_SaveDelay(5000),
  m_LastChangedBy(NULL)
{
  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(performSave()));

  m_Timer.setSingleShot(false);

  m_Timer.start(m_SaveDelay);
}

QxrdSettingsSaver::~QxrdSettingsSaver()
{
  //  performSave();
}

void QxrdSettingsSaver::performSave()
{
  int nupdates = m_ChangeCount.fetchAndStoreOrdered(0);

  if (nupdates > 0) {
    if (qcepDebug(DEBUG_PREFS)) {
      g_Application->printMessage(tr("Settings Saver saving %1 updates").arg(nupdates));

      if (m_LastChangedBy) {
        g_Application->printMessage(tr("Last changed by %1").arg(m_LastChangedBy->name()));
      }
    }

    m_LastChangedBy = NULL;

    QTime tic;
    tic.start();

    INVOKE_CHECK(QMetaObject::invokeMethod(m_Owner, "writeSettings"/*, Qt::BlockingQueuedConnection*/));

    if (qcepDebug(DEBUG_PREFS)) {
      g_Application->printMessage(tr("Saving settings took %1 msec").arg(tic.elapsed()));
    }
  }
}

void QxrdSettingsSaver::changed(QcepProperty *prop)
{
  m_ChangeCount.fetchAndAddOrdered(1);
  m_LastChangedBy = prop;
}
