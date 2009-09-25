/******************************************************************
*
*  $Id: qxrdmaskdata.h,v 1.4 2009/09/25 22:42:48 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKDATA_H
#define QXRDMASKDATA_H

#include "qcepmacros.h"
#include "qxrdimagedata.h"

class QxrdMaskData : public QxrdImageData<bool>
{
  Q_OBJECT;

public:
  QxrdMaskData(int width=0, int height=0);

public slots:
  bool maskValue(int x, int y) const;
  void setMaskValue(int x, int y, bool mval);

  void setCircularMask();
  void showMaskAll();
  void hideMaskAll();
  void invertMask();

  void maskCircle(double cx, double cy, double r, bool val);

public:
  void copyMask(QxrdMaskData *data);

  bool* mask();

  template <typename T> void setMaskRange(QcepImageData<T> *image,  T min, T max, bool inRange, bool setTo);
  template <typename T> void showMaskRange(QcepImageData<T> *image, T min, T max);
  template <typename T> void hideMaskRange(QcepImageData<T> *image, T min, T max);

private:
  HEADER_IDENT("$Id: qxrdmaskdata.h,v 1.4 2009/09/25 22:42:48 jennings Exp $");
};

template <typename T>
void QxrdMaskData::setMaskRange(QcepImageData<T> *image, T min, T max, bool inRange, bool setTo)
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
void QxrdMaskData::hideMaskRange(QcepImageData<T> *image, T min, T max)
{
  setMaskRange<T>(image, min, max, true, false);
}

template <typename T>
void QxrdMaskData::showMaskRange(QcepImageData<T> *image, T min, T max)
{
  setMaskRange<T>(image, min, max, true, true);
}

#endif // QXRDMASKDATA_H

/******************************************************************
*
*  $Log: qxrdmaskdata.h,v $
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
