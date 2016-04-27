#include "qcepdataarray.h"
#include <QScriptEngine>
#include "qcepallocator.h"
#include "qcepmutexlocker.h"

QcepDataArray::QcepDataArray(QcepObjectWPtr parent, QString name, QVector<int> dims) :
  QcepDataObject(parent, name, 0),
  m_Dimensions(dims)
{
  qint64 prod = arrayCount(dims);

  m_Data.resize(prod);

  set_ByteSize(prod*sizeof(double));

  QcepAllocator::allocate(get_ByteSize());
}

QcepDataArray::~QcepDataArray()
{
  QcepAllocator::deallocate(get_ByteSize());
}

void QcepDataArray::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepDataObject::writeSettings(settings, section);

  if (settings) {
    settings->beginWriteArray("dims");

    for (int i=0; i<m_Dimensions.count(); i++) {
      settings->setArrayIndex(i);
      settings->setValue("size", m_Dimensions.value(i));
    }

    settings->endArray();
  }
}

void QcepDataArray::readSettings(QSettings *settings, QString section)
{
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

int QcepDataArray::arrayCount(QVector<int> dims)
{
  int res=1;

  foreach(int dim, dims) {
    res *= dim;
  }

  return res;
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
      QcepObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataArrayPtr cs = qSharedPointerDynamicCast<QcepDataArray>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}
