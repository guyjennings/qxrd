/******************************************************************
*
*  $Id: qxrdmaskdata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#include "qxrdmaskdata.h"

QxrdMaskData::QxrdMaskData(QxrdAllocatorInterface *allocator, int width, int height)
  : QxrdImageData<short>(allocator, width, height),
    SOURCE_IDENT("$Id: qxrdmaskdata.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
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

/******************************************************************
*
*  $Log: qxrdmaskdata.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:40  jennings
*  Moving files into source subdirectory
*
*  Revision 1.6.4.5  2010/05/24 21:02:39  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.6.4.4  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.6.4.3  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.6.4.2  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.6.4.1  2010/04/21 21:45:03  jennings
*  Changed mask values to shorts instead of bools.  Added ROI and histogram calculation tests.
*
*  Revision 1.6  2009/09/26 04:56:37  jennings
*  Reversed direction of copyMask operation to match copyImage
*
*  Revision 1.5  2009/09/22 19:45:33  jennings
*  Small changes to range calculating code for image data
*
*  Revision 1.4  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.3  2009/08/27 17:05:59  jennings
*  Made mask data descend from QxrdImageData<bool>
*
*  Revision 1.2  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.1  2009/08/03 20:58:59  jennings
*  Minor fixups
*
*
*******************************************************************/
