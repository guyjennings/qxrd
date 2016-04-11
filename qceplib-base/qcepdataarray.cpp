#include "qcepdataarray.h"
#include <QScriptEngine>
#include "qcepallocator.h"

QcepDataArray::QcepDataArray(QcepSettingsSaverWPtr saver, QString name, QVector<int> dims) :
  QcepDataObject(saver, name, 0),
  m_Dimensions(dims)
{
  set_Type("Data Array");

  int prod = 1;
  for (int i=0; i<m_Dimensions.count(); i++) {
    prod *= m_Dimensions[i];
  }

  m_Data.resize(prod);

  set_ByteSize(prod);

  QcepAllocator::allocate(get_ByteSize());
}

QcepDataArray::~QcepDataArray()
{
  QcepAllocator::deallocate(get_ByteSize());
}

QString QcepDataArray::description() const
{
  QString desc = "[";

  for (int i=0; i<m_Dimensions.count(); i++) {
    if (i!=0) {
      desc += ", ";
    }

    desc += tr("%1").arg(m_Dimensions[i]);
  }

  desc += "]";

  return desc;
}

QVector<int> QcepDataArray::dimensions()
{
  return m_Dimensions;
}

QVector<double> QcepDataArray::vectorData()
{
  return m_Data;
}

QScriptValue QcepDataArray::toArrayScriptValue(QScriptEngine *engine, const QcepDataArrayPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataArray::fromArrayScriptValue(const QScriptValue &obj, QcepDataArrayPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataArray *qdobj = qobject_cast<QcepDataArray*>(qobj);

    if (qdobj) {
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataArrayPtr cs = qSharedPointerDynamicCast<QcepDataArray>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}
