#include "qxrddebug.h"
#include "qxrdrasterdata.h"
#include "qxrdapplication.h"
#include <QString>

QxrdRasterData::QxrdRasterData(QxrdDoubleImageDataPtr img, int interp, QxrdMaskDataPtr mask, QwtInterval range)
  : QwtRasterData(/*QRectF(0,0,(img?img->get_Width():0),(img?img->get_Height():0))*/),
    m_Data(img),
    m_Mask(mask),
    m_NRows((img ? img->get_Height(): 0)),
    m_NCols((img ? img->get_Width() : 0)),
    m_Range(range),
    m_Interpolate(interp)
{
  if (qcepDebug(DEBUG_IMAGES)) {
    if (g_Application) {
      g_Application->printMessage(QObject::tr("QxrdRasterData::QxrdRasterData(%1,%2,%3) [%4]")
                        .HEXARG(img.data()).arg(interp).HEXARG(mask.data()).HEXARG(this));
    }
  }

  setInterval(Qt::XAxis, QwtInterval(0.0, m_NCols));
  setInterval(Qt::YAxis, QwtInterval(0.0, m_NCols));
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
  if (g_Application && qcepDebug(DEBUG_IMAGES)) {
    g_Application->printMessage(QObject::tr("QxrdMaskRasterData::copy() [%1]").HEXARG((void*) this));
  }

  try {
    return new QxrdRasterData(this->data(), this->interp(), this->mask(), this->range());
  }

  catch (...) {
    if (g_Application) {
      g_Application->printMessage("QxrdRasterData::copy failed");
    }

    return NULL;
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
  int npixels = m_NRows*m_NCols;

  if (m_Data) {
    double *data = m_Data->data();
    bool first = 1;
    double min = 0;

    if (m_Mask == NULL) {
      for (int i=0; i<npixels; i++) {
        double val = data[i];
        if (first) {
          min = val;
          first = 0;
        } else if (val<min) {
          min = val;
        }
      }
    } else {
      short *mask = m_Mask->mask();

      for (int i=0; i<npixels; i++) {
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
      for (int i=0; i<npixels; i++) {
        double val = data[i];
        if (first) {
          max = val;
          first = 0;
        } else if (val>max) {
          max = val;
        }
      }
    } else {
      short *mask = m_Mask->mask();

      for (int i=0; i<npixels; i++) {
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

QwtInterval QxrdRasterData::percentileRange(double lowpct, double highpct)
{
  QwtInterval res;

  if (m_Data) {
    const int histSize = 65536;
    QVector<int> histogramVec(histSize+1);
    histogramVec.fill(0.0);
    int *histogram = histogramVec.data();

    bool first = 1;
    double minVal=0, maxVal=0;
    int npixels = m_NRows*m_NCols;
    double histStep=0;
    int nAbove = 0, nBelow = 0, nTotal = 0;

    double *data = m_Data->data();

    if (m_Mask == NULL) {
      for (int i=0; i<npixels; i++) {
        double val = data[i];
        if (first) {
          minVal = val;
          maxVal = val;
          first = 0;
        } else if (val>maxVal) {
          maxVal = val;
        } else if (val<minVal) {
          minVal = val;
        }
      }

      if (!first) {
        histStep = (maxVal - minVal + 2)/histSize;
        for (int i=0; i<npixels; i++) {
          double val = data[i];
          double bin = (val - minVal)/histStep;

          if (bin < 0) {
            nBelow += 1;
          } else if (bin > histSize) {
            nAbove += 1;
          } else {
            histogram[((int)bin)] += 1;
          }

          nTotal += 1;
        }
      }
    } else {
      short *mask = m_Mask->mask();

      for (int i=0; i<npixels; i++) {
        bool msk = mask[i];
        if (msk) {
          double val = data[i];
          if (first) {
            minVal = val;
            maxVal = val;
            first = 0;
          } else if (val>maxVal) {
            maxVal = val;
          } else if (val<minVal) {
            minVal = val;
          }
        }
      }

      if (!first) {
        histStep = (maxVal - minVal + 2)/histSize;
        for (int i=0; i<npixels; i++) {
          bool msk = mask[i];
          if (msk) {
            double val = data[i];
            double bin = (val - minVal)/histStep;

            if (bin < 0) {
              nBelow += 1;
            } else if (bin > histSize) {
              nAbove += 1;
            } else {
              histogram[((int)bin)] += 1;
            }

            nTotal += 1;
          }
        }
      }
    }

    double lowCount = ((double) nTotal) * lowpct / 100.0;
    double highCount = ((double) nTotal) * highpct / 100.0;
    double count = nBelow;

    for (int i=0; i<histSize; i++) {
      double binVal = minVal + i*histStep;

//      if (histogram[i]>100) {
//        printf("%g\t%d\t%g\n", binVal, histogram[i], count);
//      }

      count += histogram[i];

      if (count < lowCount) {
        res.setMinValue(binVal);
      }

      if (count < highCount) {
        res.setMaxValue(binVal);
      }
    }
  }

  if (res.maxValue() <= res.minValue()) {
    res.setMaxValue(res.minValue()+1);
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
