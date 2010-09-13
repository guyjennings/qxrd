/******************************************************************
*
*  $Id: qxrdmaskdata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKDATA_H
#define QXRDMASKDATA_H

#include "qcepmacros.h"
//#include "qxrdforwardtypes.h"
#include "qxrdimagedata.h"

class QxrdMaskData;

typedef QSharedPointer<QxrdMaskData>             QxrdMaskDataPtr;

class QxrdMaskData : public QxrdImageData<short>
{
  Q_OBJECT;

public:
  QxrdMaskData(QxrdAllocatorInterface *allocator, int width=0, int height=0);

public slots:
  bool maskValue(int x, int y) const;
  void setMaskValue(int x, int y, bool mval);

  void setCircularMask();
  void showMaskAll();
  void hideMaskAll();
  void invertMask();

  void maskCircle(double cx, double cy, double r, bool val);

public:
  void copyMask(QxrdMaskDataPtr dest);

  short* mask();

  template <typename T> void setMaskRange(QSharedPointer< QcepImageData<T> > image,  T min, T max, bool inRange, bool setTo);
  template <typename T> void showMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max);
  template <typename T> void hideMaskRange(QSharedPointer< QcepImageData<T> > image, T min, T max);

private:
  HEADER_IDENT("$Id: qxrdmaskdata.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
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

/******************************************************************
*
*  $Log: qxrdmaskdata.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:40  jennings
*  Moving files into source subdirectory
*
*  Revision 1.5.4.5  2010/06/14 20:57:21  jennings
*  *** empty log message ***
*
*  Revision 1.5.4.4  2010/05/24 21:02:39  jennings
*  Moved all image data allocation into allocator object
*  Added partial handling for insufficient memory available when allocating data
*  Reordered program initialization so that allocator and file saver are created first
*
*  Revision 1.5.4.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.5.4.2  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.5.4.1  2010/04/21 21:45:03  jennings
*  Changed mask values to shorts instead of bools.  Added ROI and histogram calculation tests.
*
*  Revision 1.5  2009/09/26 04:56:37  jennings
*  Reversed direction of copyMask operation to match copyImage
*
*  Revision 1.4  2009/09/25 22:42:48  jennings
*  Masking changes
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
