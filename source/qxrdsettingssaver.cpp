/******************************************************************
*
*  $Id: qxrdsettingssaver.cpp,v 1.2 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#include "qxrdsettingssaver.h"
#include "qcepproperty.h"
#include "qxrdapplication.h"

QxrdSettingsSaver::QxrdSettingsSaver(QObject *parent, QxrdApplicationPtr app) :
    QObject(parent),
    m_Application(app),
    m_SaveDelay(5000),
    SOURCE_IDENT("$Id: qxrdsettingssaver.cpp,v 1.2 2010/10/21 16:31:24 jennings Exp $")
{
  connect(&m_Timer, SIGNAL(timeout()), this, SLOT(performSave()));

  m_Timer.setSingleShot(false);

  m_Timer.start(m_SaveDelay);
}

void QxrdSettingsSaver::performSave()
{
  int nupdates = QcepProperty::getChangeCount();

  if (nupdates > 0) {
    emit printMessage(tr("Settings Saver saving %1 updates").arg(nupdates));

    QTime tic;
    tic.start();

    INVOKE_CHECK(QMetaObject::invokeMethod(m_Application, "writeSettings", Qt::BlockingQueuedConnection));

    emit printMessage(tr("Saving settings took %1 msec").arg(tic.elapsed()));
  }
}

/******************************************************************
*
*  $Log: qxrdsettingssaver.cpp,v $
*  Revision 1.2  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.1  2010/10/19 18:23:38  jennings
*  *** empty log message ***
*
*
*******************************************************************/

