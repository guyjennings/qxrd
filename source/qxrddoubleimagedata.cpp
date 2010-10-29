#include "qxrddoubleimagedata.h"

QxrdDoubleImageData::QxrdDoubleImageData(QxrdAllocatorInterface *allocator, int width, int height)
  : QxrdImageData<double>(allocator, width, height),
  m_Mask(NULL),
  m_Overflow(NULL)
{
}

void QxrdDoubleImageData::setMask(QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow)
{
  m_Mask     = mask;
  m_Overflow = overflow;
}

QxrdMaskDataPtr QxrdDoubleImageData::mask() const
{
  return m_Mask;
}

QxrdMaskDataPtr QxrdDoubleImageData::overflow() const
{
  return m_Overflow;
}

double QxrdDoubleImageData::findMin() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  double minv = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        double val = value(col, row);

        if (first) {
          minv = val;
          first = false;
        } else if (val < minv){
          minv = val;
        }
      }
    }
  }

  return minv;
}

double QxrdDoubleImageData::findMax() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  int first = true;
  double maxv = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        double val = value(col, row);

        if (first) {
          maxv = val;
          first = false;
        } else if (val > maxv){
          maxv = val;
        }
      }
    }
  }

  return maxv;
}

double QxrdDoubleImageData::findAverage() const
{
  int ncols = this -> get_Width();
  int nrows = this -> get_Height();
  double npix = 0;
  double sum = 0;

  for (int row=0; row<nrows; row++) {
    for (int col=0; col<ncols; col++) {
      if (m_Mask == NULL || m_Mask->value(col,row)) {
        double val = value(col, row);
        npix += 1;
        sum += val;
      }
    }
  }

  if (npix <= 0) {
    return 0;
  } else {
    return sum/npix;
  }
}
