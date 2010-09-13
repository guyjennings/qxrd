/******************************************************************
*
*  $Id: qxrdintegrateddata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

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
    m_cy(0),
    SOURCE_IDENT("$Id: qxrdintegrateddata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
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

/******************************************************************
*
*  $Log: qxrdintegrateddata.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.3  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.1.2.2  2010/06/14 14:18:37  jennings
*  Simple implementation of QxrdIntegratedData
*
*  Revision 1.1.2.1  2010/06/11 21:20:40  jennings
*  Added QxrdSharedPointer, QxrdIntegratedData and QxrdIntegratedDataQueue
*
*
*******************************************************************/
