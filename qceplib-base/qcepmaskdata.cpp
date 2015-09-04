#include "qcepmaskdata.h"
#include "qcepallocator.h"

QcepMaskData::QcepMaskData(QcepSettingsSaverWPtr saver, int width, int height, int def, QcepObject *parent)
  : QcepImageData<short>(saver, width, height, def, parent)
{
  set_Type("Mask Data");
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

  dest -> set_Title(get_Title()+" copy");
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
    m_Image[(get_Height()-y-1)*get_Width()+x] = mval;
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
}

void QcepMaskData::hideMaskAll()
{
  m_Image.fill(0);
}

void QcepMaskData::showMaskAll()
{
  m_Image.fill(1);
}

void QcepMaskData::invertMask()
{
  int total = m_Image.count();
//  bool *msk = m_Mask.data();

  for (int i=0; i<total; i++) {
    m_Image[i] = !m_Image[i];
  }
}

QcepMaskDataPtr QcepMaskData::copyMask(QcepObject *parent)
{
  QcepMaskDataPtr copy(QcepAllocator::newMask(QcepAllocator::AlwaysAllocate, get_Width(), get_Height(), 0, parent));

  copyMaskTo(copy);

  return copy;
}

void QcepMaskData::growMask()
{
  QcepMaskDataPtr copy(copyMask(this));

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
}

void QcepMaskData::shrinkMask()
{
  QcepMaskDataPtr copy(copyMask(this));

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
}

void QcepMaskData::maskCircle(double cx, double cy, double r, bool val)
{
  for (int j=0; j<=r; j++) {
    double dy = (((double)j)/r);
    if (fabs(dy)<1.0) {
      double dx = sqrt(1-dy*dy);
      int x0 = (int) (cx - dx*r);
      int x1 = (int) (cx + dx*r);

      for (int i=x0; i<x1; i++) {
        setMaskValue(i, (int)(cy+j), val);
        setMaskValue(i, (int)(cy-j), val);
      }
    }
  }
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

QImage QcepMaskData::thumbnailImage() const
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

  return res;
}

QSize QcepMaskData::thumbnailImageSize() const
{
  return QSize(ThumbnailWidth, ThumbnailHeight);
}

template <typename T>
void QcepMaskData::setMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max, bool inRange, bool setTo)
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
void QcepMaskData::hideMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max)
{
  setMaskRange<T>(image, min, max, true, false);
}

template <typename T>
void QcepMaskData::showMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max)
{
  setMaskRange<T>(image, min, max, true, true);
}

template void QcepMaskData::hideMaskRange<double>(QSharedPointer<QcepImageData<double> >, double, double);
template void QcepMaskData::showMaskRange<double>(QSharedPointer<QcepImageData<double> >, double, double);
