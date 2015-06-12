#include "qxrdintegrateddata.h"
#include "qxrdsettingssaver.h"

QxrdIntegratedData::QxrdIntegratedData(QxrdSettingsSaverWPtr saver,
                                       QxrdAllocatorWPtr alloc,
                                       QxrdDoubleImageDataPtr image,
                                       int typ, int maxSize, QObject *parent) :
  QcepDataObject(saver, "Integrated"),
  m_Title(saver, this, "title", "", "Integrated Data Title"),
  m_ObjectCounter(alloc, typ),
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
  set_Description("0 Rows");

  m_ObjectCounter.allocate(sizeof(double), 2, m_MaxSize);
}

QxrdIntegratedData::~QxrdIntegratedData()
{
}

void QxrdIntegratedData::resize(int n)
{
  if (n >= m_MaxSize) {
    int newSize = (n/m_AllocStep+1)*m_AllocStep;
    m_X.resize(newSize);
    m_Y.resize(newSize);

    m_ObjectCounter.allocate(sizeof(double), 2, newSize-m_MaxSize);

    m_MaxSize = newSize;
  }

  m_Size = n;

  set_Description(tr("%1 Rows").arg(m_Size));
}

int QxrdIntegratedData::size() const
{
  return m_Size;
}

const double* QxrdIntegratedData::x() const
{
  return m_X.data();
}

const double* QxrdIntegratedData::y() const
{
  return m_Y.data();
}

void QxrdIntegratedData::set_Center(double cx, double cy)
{
  m_cx = cx;
  m_cy = cy;
}

double QxrdIntegratedData::cx() const
{
  return m_cx;
}

double QxrdIntegratedData::cy() const
{
  return m_cy;
}

void QxrdIntegratedData::append(double x, double y)
{
  resize(size()+1);

  m_X[size()-1] = x;
  m_Y[size()-1] = y;
}

void QxrdIntegratedData::set_Image(QxrdDoubleImageDataPtr image)
{
  m_Image = image;
}

QxrdDoubleImageDataPtr QxrdIntegratedData::get_Image() const
{
  return m_Image;
}

QString QxrdIntegratedData::get_XUnitsLabel() const
{
  return m_XUnitsLabel;
}

void QxrdIntegratedData::set_XUnitsLabel(QString units)
{
  m_XUnitsLabel = units;
}

int QxrdIntegratedData::get_Oversample() const
{
  return m_Oversample;
}

void QxrdIntegratedData::set_Oversample(int ovs)
{
  m_Oversample = ovs;
}

int QxrdIntegratedData::allocatedMemoryMB()
{
  return m_ObjectCounter.allocatedMemoryMB();
}
