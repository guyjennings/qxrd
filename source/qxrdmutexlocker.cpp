#include "qxrdmutexlocker.h"
#include <stdio.h>
#include <QThread>

QxrdMutexLocker::QxrdMutexLocker(const char *file, int line, QMutex * mutex)
  : QMutexLocker(mutex),
    m_File(file),
    m_Line(line),
    SOURCE_IDENT("$id$")
{
  m_LockTime.start();
}

QxrdMutexLocker::~QxrdMutexLocker()
{
//  if (m_LockTime.elapsed() > 100) {
//    printf("Lock held for %d msec, file %s, line %d, thread %p\n",
//           m_LockTime.elapsed(), m_File, m_Line, QThread::currentThread());
//  }
}

/******************************************************************
*
*  $Log: qxrdmutexlocker.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.2.2.1  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*
*
*******************************************************************/
