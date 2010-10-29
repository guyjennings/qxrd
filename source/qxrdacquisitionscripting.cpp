#include "qxrdacquisitionscripting.h"

#include "qxrdacquisitionthread.h"
#include <QMetaProperty>
#include "qxrdmutexlocker.h"

QxrdAcquisitionScripting::QxrdAcquisitionScripting()
  : QxrdAcquisitionParameters()
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
