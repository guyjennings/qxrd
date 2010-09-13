#ifndef QXRDMUTEXLOCKER_H
#define QXRDMUTEXLOCKER_H

#include <QMutexLocker>
#include <QTime>
#include "qcepmacros.h"

class QxrdMutexLocker : public QMutexLocker
{
public:
  QxrdMutexLocker(const char *file, int line, QMutex * mutex);
  ~QxrdMutexLocker();

private:
  QTime m_LockTime;
  const char* m_File;
  int         m_Line;
  HEADER_IDENT("$Id: qxrdmutexlocker.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDMUTEXLOCKER_H

/******************************************************************
*
*  $Log: qxrdmutexlocker.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.3  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*
*
*******************************************************************/
