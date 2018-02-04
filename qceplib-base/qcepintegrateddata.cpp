#include "qcepintegrateddata.h"
#include "qcepallocator.h"
#include <QScriptEngine>

QcepIntegratedData::QcepIntegratedData(QString name,
                                       int maxSize) :
  inherited(name, 2*maxSize*sizeof(double)),
  m_MaxSize(maxSize),
  m_Size(0),
  m_AllocStep(1024),
  m_X(maxSize),
  m_Y(maxSize),
  m_cx(0),
  m_cy(0),
  m_XUnitsLabel("TTH"),
  m_Oversample(1)
{
  QcepAllocator::allocate(sizeof(double), 2, m_MaxSize);
}

QcepIntegratedData::~QcepIntegratedData()
{
  QcepAllocator::deallocate(sizeof(double), 2, m_MaxSize);
}

void QcepIntegratedData::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);
}

QString QcepIntegratedData::description() const
{
  return tr("%1 Rows").arg(m_Size);
}

void QcepIntegratedData::resize(int n)
{
  if (n >= m_MaxSize) {
    int newSize = (n/m_AllocStep+1)*m_AllocStep;
    m_X.resize(newSize);
    m_Y.resize(newSize);

    QcepAllocator::allocate(sizeof(double), 2, newSize-m_MaxSize);

    set_ByteSize(2*newSize*sizeof(double));

    m_MaxSize = newSize;
  }

  m_Size = n;
}

int QcepIntegratedData::size() const
{
  return m_Size;
}

const double* QcepIntegratedData::x() const
{
  return m_X.data();
}

const double* QcepIntegratedData::y() const
{
  return m_Y.data();
}

double QcepIntegratedData::x(int i) const
{
  return m_X.value(i);
}

double QcepIntegratedData::y(int i) const
{
  return m_Y.value(i);
}

void QcepIntegratedData::set_Center(double cx, double cy)
{
  m_cx = cx;
  m_cy = cy;
}

double QcepIntegratedData::cx() const
{
  return m_cx;
}

double QcepIntegratedData::cy() const
{
  return m_cy;
}

void QcepIntegratedData::append(double x, double y)
{
  resize(size()+1);

  m_X[size()-1] = x;
  m_Y[size()-1] = y;
}

void QcepIntegratedData::setValue(int n, double x, double y)
{
  m_X.replace(n, x);
  m_Y.replace(n, y);
}

void QcepIntegratedData::selfNormalize(double minx, double maxx)
{
  double sumn = 0, sumv = 0;

  for (int i=0; i<m_Size; i++) {
    double x = m_X.value(i);
    double y = m_Y.value(i);

    if (x == x && y == y) {
      if (x >= minx && x <= maxx) {
        sumn += 1;
        sumv += y;
      }
    }
  }

  if (sumn >= 5) {
    double norm = sumn/sumv;

    for (int i=0; i<m_Size; i++) {
      double x = m_X.value(i);
      double y = m_Y.value(i);

      if (x == x && y == y) {
        m_Y[i] = y*norm;
      }
    }
  }
}

QString QcepIntegratedData::get_XUnitsLabel() const
{
  return m_XUnitsLabel;
}

void QcepIntegratedData::set_XUnitsLabel(QString units)
{
  m_XUnitsLabel = units;
}

int QcepIntegratedData::get_Oversample() const
{
  return m_Oversample;
}

void QcepIntegratedData::set_Oversample(int ovs)
{
  m_Oversample = ovs;
}

QScriptValue QcepIntegratedData::toIntegratedDataScriptValue(QScriptEngine *engine, const QcepIntegratedDataPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepIntegratedData::fromIntegratedDataScriptValue(const QScriptValue &obj, QcepIntegratedDataPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepIntegratedData *qdobj = qobject_cast<QcepIntegratedData*>(qobj);

    if (qdobj) {
      QcepObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepIntegratedDataPtr cs = qSharedPointerDynamicCast<QcepIntegratedData>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}
