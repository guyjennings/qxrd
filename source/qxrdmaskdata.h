#ifndef QXRDMASKDATA_H
#define QXRDMASKDATA_H

#include "qcepmacros.h"
//#include "qxrdforwardtypes.h"
#include "qcepimagedata.h"
#include <QImage>

class QxrdMaskData;
typedef QSharedPointer<QxrdMaskData>             QxrdMaskDataPtr;

class QxrdAllocatorInterface;

class QxrdMaskData : public QcepImageData<short>
{
  Q_OBJECT;

public:
  QxrdMaskData(QxrdAllocatorInterface *allocator, int width=0, int height=0, int def=0);
  ~QxrdMaskData();

public slots:
  bool maskValue(int x, int y) const;
  void setMaskValue(int x, int y, bool mval);

  void setCircularMask();
  void showMaskAll();
  void hideMaskAll();
  void invertMask();

  void andMask(QxrdMaskDataPtr mask);
  void orMask(QxrdMaskDataPtr mask);
  void xorMask(QxrdMaskDataPtr mask);
  void andNotMask(QxrdMaskDataPtr mask);
  void orNotMask(QxrdMaskDataPtr mask);
  void xorNotMask(QxrdMaskDataPtr mask);

  void maskCircle(double cx, double cy, double r, bool val);

  int countMaskedPixels() const;
  int countUnmaskedPixels() const;
  int countOverflowPixels() const;

  QString summary();

  QImage thumbnailImage() const;
  QSize  thumbnailImageSize() const;

public:
  void copyMaskTo(QxrdMaskDataPtr dest);

  short* mask();

  template <typename T> void setMaskRange(QSharedPointer< QcepImageData<T> > image,  T min, T max, bool inRange, bool setTo);
  template <typename T> void showMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max);
  template <typename T> void hideMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max);

private:
  enum {
    ThumbnailWidth = 24,
    ThumbnailHeight = 24
  };

  QxrdAllocatorInterface    *m_Allocator;
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
