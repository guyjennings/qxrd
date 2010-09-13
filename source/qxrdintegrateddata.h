/******************************************************************
*
*  $Id: qxrdintegrateddata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATEDDATA_H
#define QXRDINTEGRATEDDATA_H

#include <QObject>
#include "qxrdimagedata.h"
#include "qxrddoubleimagedata.h"

class QxrdIntegratedData : public QObject
{
  Q_OBJECT

public:
  explicit QxrdIntegratedData(QxrdAllocatorInterface *alloc, QxrdDoubleImageDataPtr data, int maxSize, QObject *parent = 0);

  void resize(int n);
  int size() const;
  void append(double x, double y);
  void set_Center(double cx, double cy);
  void set_Image(QxrdDoubleImageDataPtr image);
  QxrdDoubleImageDataPtr get_Image() const;

  const double* x() const;
  const double* y() const;

  double cx() const;
  double cy() const;

private:
  QxrdAllocatorInterface    *m_Allocator;
  QxrdDoubleImageDataPtr     m_Image;
  int                        m_MaxSize;
  int                        m_Size;
  const int                  m_AllocStep;

  QVector<double>            m_X, m_Y;
  double                     m_cx, m_cy;

  HEADER_IDENT("$Id: qxrdintegrateddata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

typedef QSharedPointer<QxrdIntegratedData>       QxrdIntegratedDataPtr;

#endif // QXRDINTEGRATEDDATA_H

/******************************************************************
*
*  $Log: qxrdintegrateddata.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.5  2010/06/17 16:02:36  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*  Revision 1.1.2.4  2010/06/14 20:34:11  jennings
*  Removed QxrdIntegratedDataPtr from qxrdforwardtypes.h
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
