#include "qxrdintegrateddata.h"

QxrdIntegratedData::QxrdIntegratedData(QxrdAllocatorInterface *alloc, QxrdDoubleImageDataPtr image, int maxSize, QObject *parent) :
    QObject(parent),
    m_Allocator(alloc),
    m_Image(image),
    m_MaxSize(maxSize),
    m_Size(0),
    m_AllocStep(1024),
    m_X(maxSize),
    m_Y(maxSize),
    m_cx(0),
    m_cy(0)
{
}

void QxrdIntegratedData::resize(int n)
{
  if (n >= m_MaxSize) {
    int newSize = (n/m_AllocStep+1)*m_AllocStep;
    m_X.resize(newSize);
    m_Y.resize(newSize);
    m_MaxSize = newSize;
  }

  m_Size = n;
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
