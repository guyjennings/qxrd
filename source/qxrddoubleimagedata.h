/******************************************************************
*
*  $Id: qxrddoubleimagedata.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDOUBLEIMAGEDATA_H
#define QXRDDOUBLEIMAGEDATA_H

#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include <QSharedPointer>

class QxrdDoubleImageData : public QxrdImageData<double>
{
  Q_OBJECT;

public:
  QxrdDoubleImageData(QxrdAllocatorInterface *allocator, int width=0, int height=0);

  typedef QxrdImageData<double> inherited;

  void setMask(QxrdMaskDataPtr mask);
  QxrdMaskDataPtr mask() const;

  template <typename T>
      void copyFrom(const QSharedPointer< QxrdImageData<T> > img);

public slots:
  double findMin() const;
  double findMax() const;
  double findAverage() const;
  double value(int ix, int iy) const { return inherited::value(ix, iy); }
  void setValue(int x, int y, double val) { inherited::setValue(x,y,val); }

private:
  QxrdMaskDataPtr m_Mask;
  HEADER_IDENT("$Id: qxrddoubleimagedata.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

template <typename T>
void QxrdDoubleImageData::copyFrom(QSharedPointer< QxrdImageData<T> > img)
{
  if (img) {
    int ncols = img -> get_Width();
    int nrows = img -> get_Height();
    int npix = ncols*nrows;

    resize(ncols, nrows);

    copyPropertiesFrom(img);

    T *srcp = img -> data();
    double *destp = data();

    for (int i=0; i<npix; i++) {
      *destp++ = *srcp++;
    }
  }
}

typedef QSharedPointer<QxrdDoubleImageData> QxrdDoubleImageDataPtr;

#endif // QXRDDOUBLEIMAGEDATA_H

/******************************************************************
*
*  $Log: qxrddoubleimagedata.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/28 20:50:15  jennings
*  Implementing routines to access image data from scripts
*  Implement min max and average routines for data
*  Changed 'data', 'dark' and 'mask' script functions to
*  dynamically return correct objects
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*
*******************************************************************/

