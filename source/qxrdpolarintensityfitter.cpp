#include "qxrdpolarintensityfitter.h"
#include "qcepimagedata.h"
#include <math.h>
#include <qmatrix4x4.h>

QxrdPolarIntensityFitter::QxrdPolarIntensityFitter(QcepDoubleImageDataPtr data, int colNum) :
  m_Data(data),
  m_ColNum(colNum),
  m_FittedAverage(0.0),
  m_FittedAmplitude(0.0)
{
}

QxrdPolarIntensityFitter::~QxrdPolarIntensityFitter()
{
}

double QxrdPolarIntensityFitter::fittedAverage()
{
  return m_FittedAverage;
}

double QxrdPolarIntensityFitter::fittedAmplitude()
{
  return m_FittedAmplitude;
}

int QxrdPolarIntensityFitter::fit()
{
  if (m_Data) {
    int nRows = m_Data->get_Height();

    double vStart = m_Data->get_VStart();
    double vStep  = m_Data->get_VStep();

    double sumnn = 0, sumnx = 0, sumxx = 0, sumva = 0, sumvb = 0;

    for (int i=0; i<nRows; i++) {
      double x = vStart + i*vStep;
      double a = 1;
      double b = cos(x*M_PI/180.0);
      double v = m_Data->value(m_ColNum, i);

      if (v == v) {
        sumnn += 1;
        sumnx += x;
        sumxx += x*x;

        sumva += v*a;
        sumvb += v*b;
      }
    }

    if (sumnn > 5) {
      QMatrix4x4 m; // Initialized as unit matrix

      m(0,0) = sumnn;
      m(1,0) = sumnx;
      m(0,1) = sumnx;
      m(1,1) = sumxx;

      bool invertible;

      QMatrix4x4 inv = m.inverted(&invertible);

      if (invertible) {
        m_FittedAverage   = inv(0,0)*sumva + inv(0,1)*sumvb;
        m_FittedAmplitude = inv(1,0)*sumva + inv(1,1)*sumvb;
      } else {
        m_FittedAverage   = sumva/sumnn;
        m_FittedAmplitude = 0;
      }
    }
  }
}

