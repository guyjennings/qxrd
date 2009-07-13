/******************************************************************
*
*  $Id: qxrdacquisitionscripting.cpp,v 1.7 2009/07/13 23:19:37 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionscripting.h"

#include "qxrdacquisitionthread.h"
#include <QMetaProperty>

QxrdAcquisitionScripting::QxrdAcquisitionScripting(QxrdDataProcessor *proc)
  : QxrdAcquisitionParameters(proc),
    m_Mutex(QMutex::Recursive),
    SOURCE_IDENT("$Id: qxrdacquisitionscripting.cpp,v 1.7 2009/07/13 23:19:37 jennings Exp $")
{
}

void QxrdAcquisitionScripting::message(QString msg)
{
  emit printMessage(msg);
  emit statusMessage(msg);
}

void QxrdAcquisitionScripting::propertyList()
{
  QMutexLocker lock(&m_Mutex);

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

