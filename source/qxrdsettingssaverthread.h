/******************************************************************
*
*  $Id: qxrdsettingssaverthread.h,v 1.2 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSETTINGSSAVERTHREAD_H
#define QXRDSETTINGSSAVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdsettingssaver.h"
#include "qxrdforwardtypes.h"
#include <QThread>

class QxrdSettingsSaverThread : public QThread
{
  Q_OBJECT;
public:
  QxrdSettingsSaverThread(QxrdApplicationPtr app);

  QxrdSettingsSaver *settingsSaver();

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

protected:
  void run();

private:
  QxrdApplicationPtr   m_Application;
  QxrdSettingsSaverPtr m_SettingsSaver;

  HEADER_IDENT("$Id: qxrdsettingssaverthread.h,v 1.2 2010/10/21 16:31:24 jennings Exp $");
};

#endif // QXRDSETTINGSSAVERTHREAD_H

/******************************************************************
*
*  $Log: qxrdsettingssaverthread.h,v $
*  Revision 1.2  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.1  2010/10/19 18:23:38  jennings
*  *** empty log message ***
*
*
*******************************************************************/

