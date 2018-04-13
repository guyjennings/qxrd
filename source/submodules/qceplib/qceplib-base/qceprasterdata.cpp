#include "qceprasterdata.h"
#include <QString>
#include "qcepimageplotwidgetsettings.h"

QcepRasterData::QcepRasterData(QcepImageDataBaseWPtr           img,
                               QcepImagePlotWidgetSettingsWPtr set)
  : QwtRasterData(),
    m_ImageData(img),
    m_Settings(set),
    m_Range(QwtInterval(0,40000))
{
  setIntervals();
}

void QcepRasterData::setIntervals()
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

double QcepRasterData::value(double x, double y) const
{
  QcepImageDataBasePtr data(m_ImageData);
  QcepImagePlotWidgetSettingsPtr set(m_Settings);

  if (data) {
    if (x < 0 || x > m_NCols) return 0;
    if (y < 0 || y > m_NRows) return 0;

    if (set && set->get_InterpolatePixels()) {
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

QwtInterval QcepRasterData::range() const
{
  return m_Range;
}

void QcepRasterData::setDisplayedRange(double min, double max)
{
  m_Range = QwtInterval(min, max);

  setInterval(Qt::ZAxis, m_Range);
}

void QcepRasterData::setImage(QcepImageDataBasePtr img)
{
  m_ImageData = img;

  setIntervals();
}

double QcepRasterData::minValue()
{
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    return data->minValue();
  } else {
    return 0;
  }
}

double QcepRasterData::maxValue()
{
  QcepImageDataBasePtr data(m_ImageData);

  if (data) {
    return data->maxValue();
  } else {
    return 0;
  }
}

QwtInterval QcepRasterData::percentileRange(double lowpct, double highpct)
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

int QcepRasterData::width() const
{
  return m_NCols;
}

int QcepRasterData::height() const
{
  return m_NRows;
}

QPointF QcepRasterData::optimizePeakPosition(QPointF pt) const
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
