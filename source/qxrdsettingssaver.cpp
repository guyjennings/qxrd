#include "qcepmacros.h"
#include "qxrdsettingssaver.h"
#include "qcepproperty.h"
#include "qxrdapplication.h"
#include <stdio.h>

QxrdSettingsSaver::QxrdSettingsSaver(QObject *owner) :
  QObject(),
  m_Owner(owner),
  m_SaveDelay(5000),
  m_LastChangedBy(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSettingsSaver::QxrdSettingsSaver(%p)\n", this);
  }

  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(performSave()));
}

QxrdSettingsSaver::~QxrdSettingsSaver()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSettingsSaver::~QxrdSettingsSaver(%p)\n", this);
  }
}

void QxrdSettingsSaver::start()
{
  m_Timer.setSingleShot(false);

  m_Timer.start(m_SaveDelay);
}

void QxrdSettingsSaver::performSave()
{
  int nupdates = m_ChangeCount.fetchAndStoreOrdered(0);

  if (nupdates > 0) {
    QMutexLocker lock(&m_Mutex);

    if (qcepDebug(DEBUG_PREFS)) {
      printMessage(tr("Settings Saver saving %1 updates").arg(nupdates));

      if (m_LastChangedBy) {
        printMessage(tr("Last changed by %1").arg(m_LastChangedBy->name()));
      }
    }

    m_LastChangedBy = NULL;

    QTime tic;
    tic.start();

    INVOKE_CHECK(QMetaObject::invokeMethod(m_Owner, "writeSettings", Qt::DirectConnection));

    if (qcepDebug(DEBUG_PREFS)) {
      printMessage(tr("Saving settings took %1 msec").arg(tic.elapsed()));
    }
  }
}

void QxrdSettingsSaver::changed(QcepProperty *prop)
{
  m_ChangeCount.fetchAndAddOrdered(1);
  m_LastChangedBy = prop;
}

void QxrdSettingsSaver::printMessage(QString msg, QDateTime ts)
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Owner, "printMessage", Q_ARG(QString, msg), Q_ARG(QDateTime, ts)));
}
