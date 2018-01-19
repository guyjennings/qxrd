#ifndef QCEPMASKDATA_H
#define QCEPMASKDATA_H

#include "qceplib_global.h"
#include "qcepimagedata.h"
#include "qcepmaskdata-ptr.h"

class QCEP_EXPORT QcepMaskData : public QcepImageData<short>
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepMaskData(QString name,
      int width = 0,
      int height = 0,
      int def = true);
  ~QcepMaskData();

public slots:
  bool maskValue(int x, int y) const;

  QcepMaskDataPtr copyMask();

  void setCircularMask();
  void showMaskAll();
  void hideMaskAll();
  void invertMask();

  void growMask();
  void shrinkMask();

  void andMask(QcepMaskDataPtr mask);
  void orMask(QcepMaskDataPtr mask);
  void xorMask(QcepMaskDataPtr mask);
  void andNotMask(QcepMaskDataPtr mask);
  void orNotMask(QcepMaskDataPtr mask);
  void xorNotMask(QcepMaskDataPtr mask);

  void maskCircle(double cx, double cy, double r, bool val);
  void maskPolygon(QVector<QPointF> poly, bool newVal);

  int countMaskedPixels() const;
  int countUnmaskedPixels() const;
  int countOverflowPixels() const;

  QString summary();

  QImage thumbnailImage();
  QSize  thumbnailImageSize() const;
  void   thumbnailInvalid();

private:
  void setMaskValue(int x, int y, bool mval);

  bool   m_ThumbnailCached;
  QImage m_ThumbnailImage;

  void   cachedThumbnail();

public:
  void copyMaskTo(QcepMaskDataPtr dest);

  short* mask();

  void setMaskRange(QcepImageDataBasePtr image,  double min, double max, bool inRange, bool setTo);
  void showMaskRange(QcepImageDataBasePtr image, double min, double max);
  void hideMaskRange(QcepImageDataBasePtr image, double min, double max);

private:
  enum {
    ThumbnailWidth = 24,
    ThumbnailHeight = 24
  };
};

Q_DECLARE_METATYPE(QcepMaskData*)

#endif // QCEPMASKDATA_H
