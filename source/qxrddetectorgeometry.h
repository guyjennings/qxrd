#ifndef QXRDDETECTORGEOMETRY_H
#define QXRDDETECTORGEOMETRY_H

#include "qcepmacros.h"

#include <QObject>

class QxrdDetectorGeometry : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdDetectorGeometry(QObject *parent = 0);

public slots:
  double convertEnergyToWavelength(double energy) const;
  double convertWavelengthToEnergy(double wavelength) const;
  double convertTwoThetaToQ(double twoTheta,double wavelength) const;
  double convertQToTwoTheta(double Q, double wavelength) const;

  double getTwoTheta(double xCenter,double yCenter,
                   double distance,double xPixel,double yPixel,
                   double pixelLength,double pixelHeight,
                   double cos_beta,double sin_beta,
                   double cos_rotation,double sin_rotation) const;

  void getTwoThetaChi(double xCenter,double yCenter,
                      double distance,double xPixel,double yPixel,
                      double pixelLength,double pixelHeight,
                      double rotation,double cos_beta,double sin_beta,
                      double cos_alpha,double sin_alpha,
                      double cos_rotation,double sin_rotation,
                      double *twoTheta,double *chi) const;

  void getQChi(double xCenter,double yCenter,double distance,
               double energy,double xPixel,double yPixel,
               double pixelLength,double pixelHeight,
               double rotation,double cos_beta,double sin_beta,
               double cos_alpha,double sin_alpha,
               double cos_rotation,double sin_rotation,
               double *q,double *chi) const;

  void getXY(double xCenter,double yCenter,double distance,
             double energy,double q,double chi,double pixelLength,
             double pixelHeight,double rotation,double cos_beta,
             double sin_beta,double cos_alpha,double sin_alpha,
             double cos_rotation,double sin_rotation,
             double * xPixel,double * yPixel) const;
};

#endif // QXRDDETECTORGEOMETRY_H
