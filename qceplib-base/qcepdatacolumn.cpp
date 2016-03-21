#include "qcepdatacolumn.h"
#include "qcepallocator.h"
#include <QScriptEngine>

QcepDataColumn::QcepDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts, QcepObject *parent, ColumnType colType, int col1, int col2) :
  QcepDataObject(saver, name, npts*sizeof(double), parent),
  m_ColumnType(saver, this, "columnType", colType, "Column Type"),
  m_Column1(saver, this, "column1", col1, "1st dependent column"),
  m_Column2(saver, this, "column2", col2, "2nd dependent column"),
  m_NPoints(npts),
  m_Vector(npts),
  m_Formatter(NULL)
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

QcepDataColumnPtr QcepDataColumn::newDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts, QcepObject *parent, ColumnType colType, int col1, int col2)
{
  QcepDataColumnPtr res(new QcepDataColumn(saver, name, npts, parent, colType, col1, col2));

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

QcepDataColumnFormatter QcepDataColumn::formatter()
{
  return m_Formatter;
}

void QcepDataColumn::setFormatter(QcepDataColumnFormatter f)
{
  m_Formatter = f;
}

void QcepDataColumn::add(QcepDataColumnPtr col)
{
  if (col) {
    double *dst = data();
    double *src = col->data();

    if (src && dst) {
      int nR = qMin(rowCount(), col->rowCount());

      for (int i=0; i<nR; i++) {
        dst[i] += src[i];
      }

      emit dataObjectChanged();
    }
  }
}

void QcepDataColumn::subtract(QcepDataColumnPtr col)
{
  if (col) {
    double *dst = data();
    double *src = col->data();

    if (src && dst) {
      int nR = qMin(rowCount(), col->rowCount());

      for (int i=0; i<nR; i++) {
        dst[i] -= src[i];
      }

      emit dataObjectChanged();
    }
  }
}

void QcepDataColumn::copy(QcepDataColumnPtr col)
{
  if (col) {
    double *dst = data();
    double *src = col->data();

    if (src && dst) {
      int nR = qMin(rowCount(), col->rowCount());

      for (int i=0; i<nR; i++) {
        dst[i] = src[i];
      }

      emit dataObjectChanged();
    }
  }
}

void QcepDataColumn::multiply(QcepDataColumnPtr col)
{
  if (col) {
    double *dst = data();
    double *src = col->data();

    if (src && dst) {
      int nR = qMin(rowCount(), col->rowCount());

      for (int i=0; i<nR; i++) {
        dst[i] *= src[i];
      }

      emit dataObjectChanged();
    }
  }
}

void QcepDataColumn::divide(QcepDataColumnPtr col)
{
  if (col) {
    double *dst = data();
    double *src = col->data();

    if (src && dst) {
      int nR = qMin(rowCount(), col->rowCount());

      for (int i=0; i<nR; i++) {
        dst[i] /= src[i];
      }

      emit dataObjectChanged();
    }
  }
}

void QcepDataColumn::add(double val)
{
  double *dst = data();
  int    nR = rowCount();

  if (dst) {
    for (int i=0; i<nR; i++) {
      dst[i] += val;
    }
  }
}

void QcepDataColumn::subtract(double val)
{
  double *dst = data();
  int    nR = rowCount();

  if (dst) {
    for (int i=0; i<nR; i++) {
      dst[i] -= val;
    }
  }
}

void QcepDataColumn::copy(double val)
{
  double *dst = data();
  int    nR = rowCount();

  if (dst) {
    for (int i=0; i<nR; i++) {
      dst[i] = val;
    }
  }
}

void QcepDataColumn::multiply(double val)
{
  double *dst = data();
  int    nR = rowCount();

  if (dst) {
    for (int i=0; i<nR; i++) {
      dst[i] *= val;
    }
  }
}

void QcepDataColumn::divide(double val)
{
  double *dst = data();
  int    nR = rowCount();

  if (dst) {
    for (int i=0; i<nR; i++) {
      dst[i] /= val;
    }
  }
}

