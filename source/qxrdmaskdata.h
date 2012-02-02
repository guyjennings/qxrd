#ifndef QXRDMASKDATA_H
#define QXRDMASKDATA_H

#include "qcepimagedata.h"
#include "qxrdimagedataobjectcounter.h"

#include <QImage>

class QxrdMaskData;
typedef QSharedPointer<QxrdMaskData>             QxrdMaskDataPtr;
class QxrdAllocatorInterface;

class QxrdMaskData : public QcepImageData<short>
{
  Q_OBJECT

public:
  QxrdMaskData(QxrdSettingsSaver *saver, QxrdAllocatorPtr allocator, int typ, int width, int height, int def=0);
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

  int allocatedMemoryMB();

private:
  enum {
    ThumbnailWidth = 24,
    ThumbnailHeight = 24
  };

  QxrdImageDataObjectCounter m_ObjectCounter;
};

#endif // QXRDMASKDATA_H
