/******************************************************************
*
*  $Id: qxrdacquisitionscripting.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionscripting.h"

#include "qxrdacquisitionthread.h"
#include <QMetaProperty>
#include "qxrdmutexlocker.h"

QxrdAcquisitionScripting::QxrdAcquisitionScripting(/*QxrdDataProcessor *proc*/)
  : QxrdAcquisitionParameters(/*proc*/),
    SOURCE_IDENT("$Id: qxrdacquisitionscripting.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
}

void QxrdAcquisitionScripting::message(QString msg)
{
  emit statusMessage(msg);
}

void QxrdAcquisitionScripting::propertyList()
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  const QMetaObject *meta = metaObject();

  int count = meta->propertyCount();
  int offset = 1; /*meta->propertyOffset();*/

  for (int i=offset; i<count; i++) {
    QMetaProperty metaproperty = meta->property(i);

    const char *name = metaproperty.name();
    QVariant value = property(name);

    emit printMessage(tr("Property %1: %2 = %3").arg(i).arg(name).arg(value.toString()));
  }
}

/******************************************************************
*
*  $Log: qxrdacquisitionscripting.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.9.2.2  2010/05/24 21:02:37  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.9.2.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.9  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.8  2009/09/21 19:51:11  jennings
*  Added call to statusMessage to criticalMessage and call printMessage from statusMessage
*
*  Revision 1.7  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.6  2009/07/09 01:15:09  jennings
*  Added some locks
*
*  Revision 1.5  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

