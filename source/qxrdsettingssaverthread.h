/******************************************************************
*
*  $Id: qxrdsettingssaverthread.h,v 1.1 2010/10/19 18:23:38 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSETTINGSSAVERTHREAD_H
#define QXRDSETTINGSSAVERTHREAD_H

#include "qcepmacros.h"
#include <QThread>

class QxrdSettingsSaverThread : public QThread
{
  Q_OBJECT;
public:
  QxrdSettingsSaverThread();

protected:
  void run();

private:
  HEADER_IDENT("$Id: qxrdsettingssaverthread.h,v 1.1 2010/10/19 18:23:38 jennings Exp $");
};

#endif // QXRDSETTINGSSAVERTHREAD_H

/******************************************************************
*
*  $Log: qxrdsettingssaverthread.h,v $
*  Revision 1.1  2010/10/19 18:23:38  jennings
*  *** empty log message ***
*
*
*******************************************************************/

