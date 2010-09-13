/*******************************************************************
*
*     $Id: qcepmacros.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

#ifndef QCEPMACROS_H
#define QCEPMACROS_H

#include <QVector>
#include <QList>
#include <QMetaType>

#include "qcepdebug.h"

#define THREAD_CHECK Q_ASSERT(QThread::currentThread() == thread())

#define INVOKE_CHECK(res) if(!res) { printf("Invoke failed File %s, Line %d\n", __FILE__, __LINE__); }

#define HEADER_IDENT(id) \
private: const char *__identfunc() { return id; } \
  const char *__identvar1; \
  const char *__identvar2;

#define SOURCE_IDENT(id) \
  __identvar1(__identfunc()), \
  __identvar2(id)

typedef QVector<double> QcepDoubleVector;
Q_DECLARE_METATYPE(QcepDoubleVector);
typedef QVector<bool> QcepBoolVector;
Q_DECLARE_METATYPE(QcepBoolVector);
typedef QVector<int> QcepIntVector;
Q_DECLARE_METATYPE(QcepIntVector);
typedef QVector<QString> QcepStringVector;
Q_DECLARE_METATYPE(QcepStringVector);

typedef QList<double> QcepDoubleList;
Q_DECLARE_METATYPE(QcepDoubleList);
typedef QList<bool> QcepBoolList;
Q_DECLARE_METATYPE(QcepBoolList);
typedef QList<int> QcepIntList;
Q_DECLARE_METATYPE(QcepIntList);
typedef QList<QString> QcepStringList;
Q_DECLARE_METATYPE(QcepStringList);

#endif // QCEPMACROS_H

/*******************************************************************
*
*     $Log: qcepmacros.h,v $
*     Revision 1.2  2010/09/13 20:00:25  jennings
*     Merged
*
*     Revision 1.1.2.3  2010/08/17 19:20:50  jennings
*     Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*     Revision 1.1.2.2  2010/05/17 15:59:53  jennings
*     Changed debugging output to use QCEP_DEBUG macro
*
*     Revision 1.1.2.1  2010/04/13 19:29:12  jennings
*     Added qceplib to cvs
*
*     Revision 1.2  2009/06/30 21:35:31  jennings
*     Debugging meta data static init problems
*
*     Revision 1.1  2009/06/27 05:06:20  jennings
*     qavrgmacros.h renamed to qcepmacros.h
*
*
*******************************************************************/
