/******************************************************************
*
*  $Id: qxrdsettingssaver.h,v 1.2 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSETTINGSSAVER_H
#define QXRDSETTINGSSAVER_H

#include "qcepmacros.h"
#include <QObject>
#include <QTimer>
#include <QMutex>

#include "qxrdforwardtypes.h"

class QxrdSettingsSaver : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdSettingsSaver(QObject *parent, QxrdApplicationPtr app);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void performSave();

private:
  QMutex             m_Mutex;
  QxrdApplicationPtr m_Application;
  QAtomicInt         m_UpdateCount;
  QTimer             m_Timer;
  int                m_SaveDelay;

  HEADER_IDENT("$Id: qxrdsettingssaver.h,v 1.2 2010/10/21 16:31:24 jennings Exp $");
};

#endif // QXRDSETTINGSSAVER_H

/******************************************************************
*
*  $Log: qxrdsettingssaver.h,v $
*  Revision 1.2  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
*  Revision 1.1  2010/10/19 18:23:38  jennings
*  *** empty log message ***
*
*
*******************************************************************/

