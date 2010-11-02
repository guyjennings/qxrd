#include "qxrdmaskdata.h"

QxrdMaskData::QxrdMaskData(QxrdAllocatorInterface *allocator, int width, int height)
  : QxrdImageData<short>(allocator, width, height)
{
  fill(1);
}

short *QxrdMaskData::mask()
{
  return m_Image.data();
}

void QxrdMaskData::copyMask(QxrdMaskDataPtr dest)
{
  int height = get_Height();
  int width  = get_Width();

  for (int j=0; j<height; j++) {
    for (int i=0; i<width; i++) {
      dest -> setMaskValue(i,j, maskValue(i,j));
    }
  }
//
//  setMask(from -> m_Mask);
}

bool QxrdMaskData::maskValue(int x, int y) const
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    return m_Image.value((get_Height()-y-1)*get_Width()+x);
  }

  return 1;
}

void QxrdMaskData::setMaskValue(int x, int y, bool mval)
{
  if (x >= 0 && x < get_Width() && y >= 0 && y < get_Height()) {
    m_Image[(get_Height()-y-1)*get_Width()+x] = mval;
  }
}

void QxrdMaskData::setCircularMask()
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

void QxrdMaskData::hideMaskAll()
{
  m_Image.fill(0);
}

void QxrdMaskData::showMaskAll()
{
  m_Image.fill(1);
}

void QxrdMaskData::invertMask()
{
  int total = m_Image.count();
//  bool *msk = m_Mask.data();

  for (int i=0; i<total; i++) {
    m_Image[i] = !m_Image[i];
  }
}

void QxrdMaskData::maskCircle(double cx, double cy, double r, bool val)
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

QString QxrdMaskData::summary()
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
