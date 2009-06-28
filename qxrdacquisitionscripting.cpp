/******************************************************************
*
*  $Id: qxrdacquisitionscripting.cpp,v 1.5 2009/06/28 04:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionscripting.h"

#include "qxrdacquisitionthread.h"
#include <QMetaProperty>

QxrdAcquisitionScripting *QxrdAcquisitionScripting::g_Acquisition = NULL;

QxrdAcquisitionScripting::QxrdAcquisitionScripting(QxrdAcquisitionThread *thread)
  : QxrdAcquisitionParameters(),
    m_Mutex(QMutex::Recursive),
    m_AcquisitionThread(thread),
    SOURCE_IDENT("$Id: qxrdacquisitionscripting.cpp,v 1.5 2009/06/28 04:00:39 jennings Exp $")
{
  g_Acquisition = this;

//  printf("QxrdAcquisitionScripting::QxrdAcquisitionScripting g_Acquisition = %p\n",
//         g_Acquisition);
}

QxrdAcquisitionThread *QxrdAcquisitionScripting::acquisitionThread() const
{
  QMutexLocker lock(&m_Mutex);

  return m_AcquisitionThread;
}

QVariant QxrdAcquisitionScripting::evaluate(QString cmd)
{
//  emit printMessage(tr("Evaluate %1").arg(cmd));

  QScriptValue res = m_ScriptEngine.evaluate(cmd);

//  emit printMessage(tr("Result = %1").arg(res.toString()));

  m_AcquisitionThread -> setResult(res.toVariant());

  return res.toVariant();
}

double QxrdAcquisitionScripting::sleep(double time)
{
  acquisitionThread() -> sleep(time);

  return 42;
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
*  Revision 1.5  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

