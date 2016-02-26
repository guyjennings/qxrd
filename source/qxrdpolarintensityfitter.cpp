#include "qxrdpolarintensityfitter.h"

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
}

