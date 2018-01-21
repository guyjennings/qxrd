#include "qcepmaskdata.h"
#include "qcepallocator.h"
#include <QPainter>

QcepMaskData::QcepMaskData(QString name, int width, int height, int def)
  : QcepImageData<short>(name, width, height, def),
    m_ThumbnailCached(false)
{
}

QcepMaskData::~QcepMaskData()
{
}

short *QcepMaskData::mask()
{
  return m_Image.data();
}

void QcepMaskData::copyMaskTo(QcepMaskDataPtr dest)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      dest -> setMaskValue(i,j, maskValue(i,j));
    }
  }

  dest -> thumbnailInvalid();
  dest -> set_Name(get_Name()+" copy");
//
//  setMask(from -> m_Mask);
}

bool QcepMaskData::maskValue(int x, int y) const
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    return m_Image.value((get_Height()-y-1)*get_Width()+x);
  }

  return defaultValue();
}

void QcepMaskData::setMaskValue(int x, int y, bool mval)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    int i = (get_Height()-y-1)*get_Width()+x;

    if (i>=0 && i<m_Image.size()) {
      m_Image[i] = mval;
    }
  }
}

void QcepMaskData::setCircularMask()
{
  double cx = get_Width()/2, cy = get_Height()/2;
  for (int y=0; y<get_Height(); y++) {
    double dy = (((double)y)-cy)/cy;
    double dx = sqrt(1-dy*dy);
    int x0 = (int) (cx-dx*cx);
    int x1 = (int) (cx+dx*cx);

    for (int x=0; x<x0; x++) {
      setMaskValue(x,y,0);
    }

    for (int x=x0; x<x1; x++) {
      setMaskValue(x,y,1);
    }

    for (int x=x1; x<get_Width(); x++) {
      setMaskValue(x,y,0);
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::maskPolygon(QVector<QPointF> poly,
                               bool newVal)
{
  int nRows = get_Height();
  int nCols = get_Width();

  QImage polyImage(nCols, nRows, QImage::Format_RGB32);
  QPainter polyPainter(&polyImage);
  QPolygonF polygon;

  foreach(QPointF pt, poly) {
    polygon.append(pt);
  }

  polyPainter.setPen(Qt::white);
  polyPainter.fillRect(0,0,nCols,nRows,Qt::black);
  polyPainter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
  polyPainter.drawPolygon(poly);

  for (int j=0; j<nRows; j++) {
    for (int i=0; i<nCols; i++) {
      if (qGray(polyImage.pixel(i,j))) {
        setMaskValue(i, j, newVal);
      }
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::hideMaskAll()
{
  m_Image.fill(0);

  thumbnailInvalid();
}

void QcepMaskData::showMaskAll()
{
  m_Image.fill(1);

  thumbnailInvalid();
}

void QcepMaskData::invertMask()
{
  int total = m_Image.count();
//  bool *msk = m_Mask.data();

  for (int i=0; i<total; i++) {
    m_Image[i] = !m_Image[i];
  }

  thumbnailInvalid();
}

QcepMaskDataPtr QcepMaskData::copyMask()
{
  QcepMaskDataPtr copy =
      QcepAllocator::newMask(get_Name(), get_Width(), get_Height(), 0, QcepAllocator::NullIfNotAvailable);

  if (copy) {
    copyMaskTo(copy);
  }

  return copy;
}

void QcepMaskData::growMask()
{
  QcepMaskDataPtr copy = copyMask();

  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      short res = maskValue(i,j);
      for (int ii=-1; ii<2; ii++) {
        for (int jj=-1; jj<2; jj++) {
          res = qMin(res, (short) copy->maskValue(i+ii, j+jj));
        }
      }
      setMaskValue(i,j, res);
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::shrinkMask()
{
  QcepMaskDataPtr copy = copyMask();

  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      short res = maskValue(i,j);
      for (int ii=-1; ii<2; ii++) {
        for (int jj=-1; jj<2; jj++) {
          res = qMax(res, (short) copy->maskValue(i+ii, j+jj));
        }
      }
      setMaskValue(i,j, res);
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::andMask(QcepMaskDataPtr mask)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      setMaskValue(i,j, maskValue(i,j) & mask->maskValue(i,j));
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::orMask(QcepMaskDataPtr mask)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      setMaskValue(i,j, maskValue(i,j) | mask->maskValue(i,j));
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::xorMask(QcepMaskDataPtr mask)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      setMaskValue(i,j, maskValue(i,j) ^ mask->maskValue(i,j));
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::andNotMask(QcepMaskDataPtr mask)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      setMaskValue(i,j, maskValue(i,j) & !mask->maskValue(i,j));
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::orNotMask(QcepMaskDataPtr mask)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      setMaskValue(i,j, maskValue(i,j) | !mask->maskValue(i,j));
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::xorNotMask(QcepMaskDataPtr mask)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      setMaskValue(i,j, maskValue(i,j) ^ !mask->maskValue(i,j));
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::maskRectangle(QRectF r, bool val)
{
  int x0 = qRound(r.left());
  int x1 = qRound(r.right());
  int y0 = qRound(r.top());
  int y1 = qRound(r.bottom());

  for (int y=y0; y<=y1; y++) {
    for (int x=x0; x<=x1; x++) {
      setMaskValue(x, y, val);
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::maskCircle(double cx, double cy, double r, bool val)
{
  int x0 = qRound(cx-r);
  int x1 = qRound(cx+r);
  int y0 = qRound(cy-r);
  int y1 = qRound(cy+r);

  for (int y=y0; y<=y1; y++) {
    for (int x=x0; x<=x1; x++) {
      double dx = x-cx;
      double dy = y-cy;
      double r0 = sqrt(dx*dx + dy*dy);

      if (r0 <= (r+1)) {
        setMaskValue(x, y, val);
      }
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::maskCircle(QRectF r, bool val)
{
  int x0 = qRound(r.left());
  int x1 = qRound(r.right());
  int y0 = qRound(r.top());
  int y1 = qRound(r.bottom());

  double cx = (r.left() + r.right())/2.0;
  double cy = (r.top() + r.bottom())/2.0;
  double rx = (r.left() - r.right())/2.0;
  double ry = (r.top() - r.bottom())/2.0;
  double rx2 = rx*rx;
  double ry2 = ry*ry;

  for (int y=y0; y<=y1; y++) {
    for (int x=x0; x<=x1; x++) {
      double dx = x-cx;
      double dy = y-cy;
      double r0 = sqrt(dx*dx/rx2 + dy*dy/ry2);

      if (r0 <= 1) {
        setMaskValue(x, y, val);
      }
    }
  }

  thumbnailInvalid();
}

QString QcepMaskData::summary()
{
  QString res;

  int total = m_Image.count();
  int totalSet = 0;

  for (int i=0; i<total; i++) {
    if (m_Image[i]) {
      totalSet++;
    }
  }

  return tr("%1/%2 pixels set").arg(totalSet).arg(total);
}

int  QcepMaskData::countMaskedPixels() const
{
  int total = m_Image.count();
  int totalSet = 0;

  for (int i=0; i<total; i++) {
    if (m_Image[i] == 0) {
      totalSet++;
    }
  }

  return totalSet;
}

int  QcepMaskData::countUnmaskedPixels() const
{
  int total = m_Image.count();
  int totalSet = 0;

  for (int i=0; i<total; i++) {
    if (m_Image[i] == 1) {
      totalSet++;
    }
  }

  return totalSet;
}

int  QcepMaskData::countOverflowPixels() const
{
  return countUnmaskedPixels();
}

QImage QcepMaskData::thumbnailImage()
{
  if (m_ThumbnailCached) {
    return m_ThumbnailImage;
  } else {
    cachedThumbnail();

    return m_ThumbnailImage;
  }
}

void QcepMaskData::thumbnailInvalid()
{
  m_ThumbnailCached = false;
}

void QcepMaskData::cachedThumbnail()
{
  int height    = get_Height();
  int width     = get_Width();
  int th_height = ThumbnailHeight;
  int th_width  = ThumbnailWidth;
  double sc_height = height/th_height;
  double sc_width  = width/th_width;
  int threshold = sc_height*sc_width;
  double scale = qMin(sc_height, sc_width);
  th_width = width/scale;
  th_height = height/scale;

  QImage res(th_width, th_height, QImage::Format_RGB32);

  for (int j=0; j<th_height; j++) {
    for (int i=0; i<th_width; i++) {
      int tot=0;
      for (int jsc = 0; jsc<sc_height; jsc++) {
        for (int isc = 0; isc<sc_width; isc++) {
          if (maskValue(i*scale+isc, j*scale+jsc)) {
            tot++;
          }
        }
      }

      int val = tot*255/(threshold+1);

      res.setPixel(i,th_height-j-1, qRgb(255-val,0,0));
    }
  }

  m_ThumbnailImage  = res;
  m_ThumbnailCached = true;
}

QSize QcepMaskData::thumbnailImageSize() const
{
  return QSize(ThumbnailWidth, ThumbnailHeight);
}

void QcepMaskData::setMaskRange(QcepImageDataBasePtr image, double min, double max, bool inRange, bool setTo)
{
  int nRows = image -> get_Height();
  int nCols = image -> get_Width();

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      double v = image -> getImageData(x,y);

      if (((v >= min) && (v < max)) == inRange) {
        setValue(x,y,setTo);
      }
    }
  }

  thumbnailInvalid();
}

void QcepMaskData::hideMaskRange(QcepImageDataBasePtr image, double min, double max)
{
  setMaskRange(image, min, max, true, false);
}

void QcepMaskData::showMaskRange(QcepImageDataBasePtr image, double min, double max)
{
  setMaskRange(image, min, max, true, true);
}
