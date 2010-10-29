#ifndef QXRDMASKDATA_H
#define QXRDMASKDATA_H

#include "qcepmacros.h"
//#include "qxrdforwardtypes.h"
#include "qxrdimagedata.h"

class QxrdMaskData;

typedef QSharedPointer<QxrdMaskData>             QxrdMaskDataPtr;

class QxrdMaskData : public QxrdImageData<short>
{
  Q_OBJECT;

public:
  QxrdMaskData(QxrdAllocatorInterface *allocator, int width=0, int height=0);

public slots:
  bool maskValue(int x, int y) const;
  void setMaskValue(int x, int y, bool mval);

  void setCircularMask();
  void showMaskAll();
  void hideMaskAll();
  void invertMask();

  void maskCircle(double cx, double cy, double r, bool val);

public:
  void copyMask(QxrdMaskDataPtr dest);

  short* mask();

  template <typename T> void setMaskRange(QSharedPointer< QcepImageData<T> > image,  T min, T max, bool inRange, bool setTo);
  template <typename T> void showMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max);
  template <typename T> void hideMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max);
};

template <typename T>
void QxrdMaskData::setMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max, bool inRange, bool setTo)
{
  int nRows = image -> get_Height();
  int nCols = image -> get_Width();

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      T v = image -> value(x,y);

      if (((v >= min) && (v < max)) == inRange) {
        setValue(x,y,setTo);
      }
    }
  }
}

template <typename T>
void QxrdMaskData::hideMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max)
{
  setMaskRange<T>(image, min, max, true, false);
}

template <typename T>
void QxrdMaskData::showMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max)
{
  setMaskRange<T>(image, min, max, true, true);
}

#endif // QXRDMASKDATA_H
