#include "qxrddebug.h"
#include "qxrdrasterdata.h"
#include "qxrdapplication.h"
#include <QString>

QxrdRasterData::QxrdRasterData(QcepImageDataBaseWPtr img, int interp)
  : QwtRasterData(),
    m_ImageData(img),
    m_Range(QwtInterval(0,40000)),
    m_Interpolate(interp)
{
  if (qcepDebug(DEBUG_IMAGES)) {
    if (g_Application) {
      g_Application->printMessage(QObject::tr("QxrdRasterData::QxrdRasterData(%1,%2) [%3]")
                        .HEXARG(img.data()).arg(interp).HEXARG(this));
    }
  }

  setIntervals();
}

void QxrdRasterData::setIntervals()
{
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    m_NRows = data->get_Height();
    m_NCols = data->get_Width();
  } else {
    m_NRows = 0;
    m_NCols = 0;
  }

  setInterval(Qt::XAxis, QwtInterval(0.0, m_NCols));
  setInterval(Qt::YAxis, QwtInterval(0.0, m_NRows));
  setInterval(Qt::ZAxis, m_Range);
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
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    if (x < 0 || x > m_NCols) return 0;
    if (y < 0 || y > m_NRows) return 0;

    if (m_Interpolate) {
      int ix = ((int) x), iy = ((int) y);
      double dx = x-ix, dy = y-iy;

      double f00 = data->getImageData((ix)   , (iy));
      double f10 = data->getImageData((ix+1) , (iy));
      double f01 = data->getImageData((ix)   , (iy+1));
      double f11 = data->getImageData((ix+1) , (iy+1));

      double f0 = f00*(1-dx)+f10*dx;
      double f1 = f01*(1-dx)+f11*dx;

      double f = f0*(1-dy)+f1*dy;

      return f;
    } else {
      return data->getImageData(((int) qRound(x)) , ((int) qRound(y)));
    }
  } else {
    return 0;
  }
}

QwtInterval QxrdRasterData::range() const
{
  return m_Range;
}

void QxrdRasterData::setDisplayedRange(double min, double max)
{
  m_Range = QwtInterval(min, max);

  setInterval(Qt::ZAxis, m_Range);
}

void QxrdRasterData::setImage(QcepImageDataBasePtr img)
{
  m_ImageData = img;

  setIntervals();
}

double QxrdRasterData::minValue()
{
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    return data->minValue();
  } else {
    return 0;
  }
}

double QxrdRasterData::maxValue()
{
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    return data->maxValue();
  } else {
    return 0;
  }
}

QwtInterval QxrdRasterData::percentileRange(double lowpct, double highpct)
{
  QwtInterval res;
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    QPointF r = data->percentileRange(lowpct, highpct);

    res.setMinValue(r.x());
    res.setMaxValue(r.y());
  }

  return res;
}

int QxrdRasterData::width() const
{
  return m_NCols;
}

int QxrdRasterData::height() const
{
  return m_NRows;
}

QPointF QxrdRasterData::optimizePeakPosition(QPointF pt) const
{
  int rad = 10;
  int ix = pt.x();
  int iy = pt.y();
  int first = 1;

  double max;
  int maxx = 0, maxy = 0;

  for (int j=iy-rad; j<iy+rad; j++) {
    for (int i=ix-rad; i<ix+rad; i++) {
      double val=value(i,j);

      if (first) {
        max = val; maxx = i; maxy = j;
        first = 0;
      } else if (val > max) {
        max = val; maxx = i; maxy = j;
      }
    }
  }

  return QPointF(maxx, maxy);
}
