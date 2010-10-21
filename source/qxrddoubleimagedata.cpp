/******************************************************************
*
*  $Id: qxrddoubleimagedata.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $
*
*******************************************************************/

#include "qxrddoubleimagedata.h"

QxrdDoubleImageData::QxrdDoubleImageData(QxrdAllocatorInterface *allocator, int width, int height)
  : QxrdImageData<double>(allocator, width, height),
  m_Mask(NULL),
  m_Overflow(NULL),
  SOURCE_IDENT("$Id: qxrddoubleimagedata.cpp,v 1.3 2010/10/21 19:44:03 jennings Exp $")
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

/******************************************************************
*
*  $Log: qxrddoubleimagedata.cpp,v $
*  Revision 1.3  2010/10/21 19:44:03  jennings
*  Adding code to display overflow pixels, removed cuda and simple processors
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/28 20:50:15  jennings
*  Implementing routines to access image data from scripts
*  Implement min max and average routines for data
*  Changed 'data', 'dark' and 'mask' script functions to
*  dynamically return correct objects
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.2.1  2010/06/17 16:02:35  jennings
*  Skeleton code to support result serialization during crucial points in processing
*  Separate QxrdDoubleImageData class which can hold a reference to a mask
*
*
*******************************************************************/

