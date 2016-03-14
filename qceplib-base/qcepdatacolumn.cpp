#include "qcepdatacolumn.h"
#include "qcepallocator.h"
#include <QScriptEngine>

QcepDataColumn::QcepDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts, QcepObject *parent) :
  QcepDataObject(saver, name, npts*sizeof(double), parent),
  m_NPoints(npts),
  m_Vector(npts)
{
  set_Type("Data Column");

  QcepAllocator::allocate(m_NPoints*sizeof(double));
}

QcepDataColumn::~QcepDataColumn()
{
  QcepAllocator::deallocate(m_NPoints*sizeof(double));
}

QString QcepDataColumn::description() const
{
  return tr("%1 rows").arg(m_NPoints);
}

QcepDataColumnPtr QcepDataColumn::newDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts, QcepObject *parent)
{
  QcepDataColumnPtr res(new QcepDataColumn(saver, name, npts, parent));

  return res;
}

QScriptValue QcepDataColumn::toColumnScriptValue(QScriptEngine *engine, const QcepDataColumnPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataColumn::fromColumnScriptValue(const QScriptValue &obj, QcepDataColumnPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataColumn *qdobj = qobject_cast<QcepDataColumn*>(qobj);

    if (qdobj) {
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataColumnPtr cs = qSharedPointerDynamicCast<QcepDataColumn>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}

int QcepDataColumn::columnCount() const
{
  return 1;
}

int QcepDataColumn::rowCount() const
{
  return m_NPoints;
}

void QcepDataColumn::resize(int n)
{
  m_Vector.resize(n);

  QcepAllocator::allocate(sizeof(double), 1, n-m_NPoints);

  m_NPoints = n;
}

double QcepDataColumn::value(int i) const
{
  return m_Vector.value(i);
}

void QcepDataColumn::setValue(int i, const double value)
{
  if (i >= 0 && i < count()) {
    m_Vector[i] = value;
  }
}

int QcepDataColumn::count() const
{
  return m_Vector.count();
}

double * QcepDataColumn::data()
{
  return m_Vector.data();
}
