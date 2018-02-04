#include "qcepdatacolumn.h"
#include "qcepallocator.h"
#include <QScriptEngine>
#include "qcepmutexlocker.h"

QcepDataColumn::QcepDataColumn(QString name, int npts, ColumnType colType, int col1, int col2) :
  inherited(name, npts*sizeof(double)),
  m_ColumnType(this, "columnType", colType, "Column Type"),
  m_Column1   (this, "column1", col1, "1st dependent column"),
  m_Column2   (this, "column2", col2, "2nd dependent column"),
  m_NPoints(npts),
  m_Vector(npts),
  m_Formatter(NULL)
{
  QcepAllocator::allocate(m_NPoints*sizeof(double));
}

QcepDataColumn::~QcepDataColumn()
{
  QcepAllocator::deallocate(m_NPoints*sizeof(double));
}

void QcepDataColumn::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);
}

void QcepDataColumn::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  if (settings) {
    settings->beginWriteArray("d");

    for (int i=0; i<m_NPoints; i++) {
      settings->setArrayIndex(i);
      settings->setValue("v", m_Vector.value(i));
    }

    settings->endArray();
  }
}

void QcepDataColumn::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

  if (settings) {
    int n = settings->beginReadArray("d");

    for (int i=0; i<n; i++) {
      settings->setArrayIndex(i);

      double v = settings->value("v").toDouble();

      append(v);
    }

    emit dataObjectChanged();

    settings->endArray();
  }
}

QString QcepDataColumn::description() const
{
  return tr("%1 rows").arg(m_NPoints);
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
      QcepObjectPtr p = qdobj->sharedFromThis();

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

void QcepDataColumn::append(double v)
{
  m_Vector.append(v);

  m_NPoints = m_Vector.count();
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

void QcepDataColumn::concat(QcepDataColumnPtr col)
{
  if (col) {
    int nr1 = rowCount();
    int nr2 = col->rowCount();

    resize(nr1 + nr2);

    double *dst = data();
    double *src = col->data();

    for (int i=0; i<nr2; i++) {
      dst[nr1+i] = src[i];
    }

    emit dataObjectChanged();
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

    emit dataObjectChanged();
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

    emit dataObjectChanged();
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

    emit dataObjectChanged();
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

    emit dataObjectChanged();
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

    emit dataObjectChanged();
  }
}

void QcepDataColumn::concat(double val)
{
  int nR = rowCount();

  resize(nR+1);

  double *dst = data();

  if (dst) {
    dst[nR] = val;

    emit dataObjectChanged();
  }
}

QcepDataColumnPtr QcepDataColumn::duplicate()
{
  QcepDataColumnPtr dup = QcepAllocator::newColumn(tr("%1.dup").arg(get_Name()), rowCount(), QcepAllocator::AlwaysAllocate);

  dup -> set_ColumnType(get_ColumnType());
  dup -> set_Column1(get_Column1());
  dup -> set_Column2(get_Column2());

  for (int i=0; i<count(); i++) {
    dup -> setValue(i, value(i));
  }

  return dup;
}
