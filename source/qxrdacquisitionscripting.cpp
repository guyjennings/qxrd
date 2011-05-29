#include "qxrdacquisitionscripting.h"

#include "qxrdacquisitionthread.h"
#include <QMetaProperty>
#include "qxrdmutexlocker.h"
#include "qxrdapplication.h"

QxrdAcquisitionScripting::QxrdAcquisitionScripting()
  : QxrdAcquisitionParameters()
{
}

void QxrdAcquisitionScripting::Message(QString msg)
{
  g_Application->statusMessage(msg);
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

    g_Application->printMessage(tr("Property %1: %2 = %3").arg(i).arg(name).arg(value.toString()));
  }
}
