#ifndef QXRDPOLARINTENSITYFITTER_H
#define QXRDPOLARINTENSITYFITTER_H

#include "qcepmacros.h"
#include "qcepimagedata-ptr.h"

class QxrdPolarIntensityFitter
{
public:
  QxrdPolarIntensityFitter(QcepDoubleImageDataPtr data, int colNum);
  ~QxrdPolarIntensityFitter();

  int fit();

  double fittedAverage();
  double fittedAmplitude();

private:
  QcepDoubleImageDataPtr m_Data;
  int                    m_ColNum;
  double                 m_FittedAverage;
  double                 m_FittedAmplitude;
};

#endif // QXRDPOLARINTENSITYFITTER_H
