/******************************************************************
*
*  $Id: qxrdrasterdata.cpp,v 1.16 2010/04/23 20:18:31 jennings Exp $
*
*******************************************************************/

#include "qxrdrasterdata.h"

QxrdRasterData::QxrdRasterData(QxrdDoubleImageData *img, int interp, QxrdMaskData *mask)
  : QwtRasterData(QwtDoubleRect(0,0,img->get_Width(),img->get_Height())),
    m_Data(img),
    m_Mask(mask),
    m_NRows((img ? img->get_Height(): 0)),
    m_NCols((img ? img->get_Width() : 0)),
    m_Range(40000,48000),
    m_Interpolate(interp),
    SOURCE_IDENT("$Id: qxrdrasterdata.cpp,v 1.16 2010/04/23 20:18:31 jennings Exp $")
{
}

QxrdRasterData::QxrdRasterData()
  : QwtRasterData(),
    m_Data(NULL),
    m_Mask(NULL),
    m_NRows(0),
    m_NCols(0),
    m_Range(0,1),
    m_Interpolate(1)
{
}

void QxrdRasterData::setInterpolate(int interp)
{
//  printf("%p->QxrdRasterData::setInterpolate(%d)\n", this, interp);

  m_Interpolate = interp;
}

int QxrdRasterData::interpolate()
{
  return m_Interpolate;
}

double QxrdRasterData::value(double x, double y) const
{
  if (m_Data) {
    if (x < 0 || x > m_NCols) return 0;
    if (y < 0 || y > m_NRows) return 0;

    if (m_Interpolate) {
      int ix = ((int) x), iy = ((int) y);
      double dx = x-ix, dy = y-iy;

      double f00 = m_Data->value((ix)   , (iy));
      double f10 = m_Data->value((ix+1) , (iy));
      double f01 = m_Data->value((ix)   , (iy+1));
      double f11 = m_Data->value((ix+1) , (iy+1));

      double f0 = f00*(1-dx)+f10*dx;
      double f1 = f01*(1-dx)+f11*dx;

      double f = f0*(1-dy)+f1*dy;

      return f;
    } else {
      return m_Data->value(((int) qRound(x)) , ((int) qRound(y)));
    }
  } else {
    return 0;
  }
}

QxrdRasterData* QxrdRasterData::copy() const
{
  return new QxrdRasterData(*this);
}

QwtDoubleInterval QxrdRasterData::range() const
{
  return m_Range;
}

void QxrdRasterData::setDisplayedRange(double min, double max)
{
  m_Range = QwtDoubleInterval(min, max);
}

double QxrdRasterData::minValue()
{
  int npixels = m_NRows*m_NCols;

  if (m_Data) {
    double *data = m_Data->data();
    bool first = 1;
    double min = 0;

    if (m_Mask == NULL) {
      for (int i=1; i<npixels; i++) {
        double val = data[i];
        if (first) {
          min = val;
          first = 0;
        } else if (val<min) {
          min = val;
        }
      }
    } else {
      bool *mask = m_Mask->mask();

      for (int i=1; i<npixels; i++) {
        bool msk = mask[i];
        if (msk) {
          double val = data[i];
          if (first) {
            min = val;
            first = 0;
          } else if (val<min) {
            min = val;
          }
        }
      }
    }

    return min;
  } else {
    return 0;
  }
}

double QxrdRasterData::maxValue()
{
  int npixels = m_NRows*m_NCols;

  if (m_Data) {
    double *data = m_Data->data();
    bool first = 1;
    double max = 0;

    if (m_Mask == NULL) {
      for (int i=1; i<npixels; i++) {
        double val = data[i];
        if (first) {
          max = val;
          first = 0;
        } else if (val>max) {
          max = val;
        }
      }
    } else {
      bool *mask = m_Mask->mask();

      for (int i=1; i<npixels; i++) {
        bool msk = mask[i];
        if (msk) {
          double val = data[i];
          if (first) {
            max = val;
            first = 0;
          } else if (val>max) {
            max = val;
          }
        }
      }
    }

    return max;
  } else {
    return 0;
  }
}

int QxrdRasterData::width() const
{
  return m_NCols;
}

int QxrdRasterData::height() const
{
  return m_NRows;
}

/******************************************************************
*
*  $Log: qxrdrasterdata.cpp,v $
*  Revision 1.16  2010/04/23 20:18:31  jennings
*  Fixed problem with swapped width and height in QxrdRasterData constructor
*
*  Revision 1.15  2009/11/02 20:19:27  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.14  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.13  2009/08/04 16:45:20  jennings
*  Moved mask data into separate class
*
*  Revision 1.12  2009/07/25 17:03:40  jennings
*  More improvements to image plotting code
*
*  Revision 1.11  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

