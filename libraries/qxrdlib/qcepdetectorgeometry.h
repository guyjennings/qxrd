#ifndef QCEPDETECTORGEOMETRY_H
#define QCEPDETECTORGEOMETRY_H

#include "qceplib_global.h"
#include "qcepmacros.h"

#include "qcepobject.h"

class QCEP_EXPORT QcepDetectorGeometry : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QcepDetectorGeometry(QString name);
  void initialize(QcepObjectWPtr parent);

public slots:
  static double convertEnergyToWavelength(double energy);
  static double convertWavelengthToEnergy(double wavelength);
  static double convertTwoThetaToQ(double twoTheta,double wavelength);
  static double convertQToTwoTheta(double Q, double wavelength);

  static double getRadius(double xCenter,double yCenter,
                   double distance,double xPixel,double yPixel,
                   double pixelLength,double pixelHeight,
                   double cos_beta,double sin_beta,
                   double cos_rotation,double sin_rotation);

  static double getTwoTheta(double xCenter,double yCenter,
                   double distance,double xPixel,double yPixel,
                   double pixelLength,double pixelHeight,
                   double cos_beta,double sin_beta,
                   double cos_rotation,double sin_rotation);

  static void getTwoThetaChi(double xCenter,double yCenter,
                      double distance,double xPixel,double yPixel,
                      double pixelLength,double pixelHeight,
                      double rotation,double cos_beta,double sin_beta,
                      double cos_alpha,double sin_alpha,
                      double cos_rotation,double sin_rotation,
                      double *twoTheta,double *chi);

  static void getQChi(double xCenter,double yCenter,double distance,
               double energy,double xPixel,double yPixel,
               double pixelLength,double pixelHeight,
               double rotation,double cos_beta,double sin_beta,
               double cos_alpha,double sin_alpha,
               double cos_rotation,double sin_rotation,
               double *q,double *chi);

  static void getXY(double xCenter,double yCenter,double distance,
             double energy,double q,double chi,double pixelLength,
             double pixelHeight,double rotation,double cos_beta,
             double sin_beta,double cos_alpha,double sin_alpha,
             double cos_rotation,double sin_rotation,
             double * xPixel,double * yPixel);
};

Q_DECLARE_METATYPE(QcepDetectorGeometry*)

#endif // QCEPDETECTORGEOMETRY_H
