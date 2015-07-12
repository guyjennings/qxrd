#include "qcepintegrateddata.h"
#include "qcepsettingssaver.h"
#include <QScriptEngine>

QcepIntegratedData::QcepIntegratedData(QcepSettingsSaverWPtr saver,
                                       QcepDoubleImageDataPtr image,
                                       int typ, int maxSize, QObject *parent) :
  QcepDataObject(saver, "Integrated"),
  m_Title(saver, this, "title", "", "Integrated Data Title"),
  m_ObjectCounter(/*alloc*/ QcepAllocatorWPtr(), typ),
  m_Image(image),
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
  set_Type("Integrated Data");

  m_ObjectCounter.allocate(sizeof(double), 2, m_MaxSize);
}

QcepIntegratedData::~QcepIntegratedData()
{
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

    m_ObjectCounter.allocate(sizeof(double), 2, newSize-m_MaxSize);

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

void QcepIntegratedData::set_Image(QcepDoubleImageDataPtr image)
{
  m_Image = image;
}

QcepDoubleImageDataPtr QcepIntegratedData::get_Image() const
{
  return m_Image;
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

int QcepIntegratedData::allocatedMemoryMB()
{
  return m_ObjectCounter.allocatedMemoryMB();
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
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepIntegratedDataPtr cs = qSharedPointerDynamicCast<QcepIntegratedData>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}
