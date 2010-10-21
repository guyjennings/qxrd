/******************************************************************
*
*  $Id: qxrdsettingssaverthread.cpp,v 1.2 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#include "qxrdsettingssaverthread.h"

QxrdSettingsSaverThread::QxrdSettingsSaverThread(QxrdApplicationPtr app) :
    QThread(),
    m_Application(app),
    m_SettingsSaver(NULL),
    SOURCE_IDENT("$Id: qxrdsettingssaverthread.cpp,v 1.2 2010/10/21 16:31:24 jennings Exp $")
{
}

QxrdSettingsSaver *QxrdSettingsSaverThread::settingsSaver()
{
  while (m_SettingsSaver == NULL) {
    QThread::msleep(500);
  }

  return m_SettingsSaver;
}

void QxrdSettingsSaverThread::run()
{
  m_SettingsSaver = new QxrdSettingsSaver(NULL, m_Application);

  connect(m_SettingsSaver,             SIGNAL(printMessage(QString)), this,            SIGNAL(printMessage(QString)));
  connect(m_SettingsSaver,             SIGNAL(statusMessage(QString)), this,            SIGNAL(statusMessage(QString)));
  connect(m_SettingsSaver,             SIGNAL(criticalMessage(QString)), this,            SIGNAL(criticalMessage(QString)));

  exec();
}

/******************************************************************
*
*  $Log: qxrdsettingssaverthread.cpp,v $
*  Revision 1.2  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.1  2010/10/19 18:23:38  jennings
*  *** empty log message ***
*
*
*******************************************************************/

