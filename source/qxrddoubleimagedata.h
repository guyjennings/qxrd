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

  void setMask(QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdMaskDataPtr mask() const;
  QxrdMaskDataPtr overflow() const;

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
  QxrdMaskDataPtr m_Overflow;
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
