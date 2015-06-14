#ifndef QXRDMASKDATA_H
#define QXRDMASKDATA_H

#include "qcepimagedata.h"
#include "qxrdimagedataobjectcounter.h"
#include <QImage>
#include "qxrdmaskdata-ptr.h"
#include "qxrdallocator-ptr.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdMaskData : public QcepImageData<short>
{
  Q_OBJECT

public:
  QxrdMaskData(QxrdSettingsSaverWPtr saver, QxrdAllocatorWPtr allocator, int typ, int width, int height, int def=0);
  ~QxrdMaskData();

public slots:
  bool maskValue(int x, int y) const;
  void setMaskValue(int x, int y, bool mval);

  QxrdMaskDataPtr copyMask();

  void setCircularMask();
  void showMaskAll();
  void hideMaskAll();
  void invertMask();

  void growMask();
  void shrinkMask();

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

Q_DECLARE_METATYPE(QxrdMaskDataPtr)

#endif // QXRDMASKDATA_H
