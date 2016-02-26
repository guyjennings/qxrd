#include "qxrddebug.h"
#include "qxrdrasterdata.h"
#include "qxrdapplication.h"
#include <QString>

QxrdRasterData::QxrdRasterData(QcepImageDataBasePtr img, int interp, QwtInterval range)
  : QwtRasterData(),
    m_Data(img),
    m_NRows((img ? img->get_Height(): 0)),
    m_NCols((img ? img->get_Width() : 0)),
    m_Range(range),
    m_Interpolate(interp)
{
  if (qcepDebug(DEBUG_IMAGES)) {
    if (g_Application) {
      g_Application->printMessage(QObject::tr("QxrdRasterData::QxrdRasterData(%1,%2) [%3]")
                        .HEXARG(img.data()).arg(interp).HEXARG(this));
    }
  }

  setInterval(Qt::XAxis, QwtInterval(0.0, m_NCols));
  setInterval(Qt::YAxis, QwtInterval(0.0, m_NRows));
  setInterval(Qt::ZAxis, range);
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

      double f00 = m_Data->getImageData((ix)   , (iy));
      double f10 = m_Data->getImageData((ix+1) , (iy));
      double f01 = m_Data->getImageData((ix)   , (iy+1));
      double f11 = m_Data->getImageData((ix+1) , (iy+1));

      double f0 = f00*(1-dx)+f10*dx;
      double f1 = f01*(1-dx)+f11*dx;

      double f = f0*(1-dy)+f1*dy;

      return f;
    } else {
      return m_Data->getImageData(((int) qRound(x)) , ((int) qRound(y)));
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

double QxrdRasterData::minValue()
{
  if (m_Data) {
    return m_Data->minValue();
  } else {
    return 0;
  }
}

double QxrdRasterData::maxValue()
{
  if (m_Data) {
    return m_Data->maxValue();
  } else {
    return 0;
  }
}

QwtInterval QxrdRasterData::percentileRange(double lowpct, double highpct)
{
  QwtInterval res;

  if (m_Data) {
    QPointF r = m_Data->percentileRange(lowpct, highpct);

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
