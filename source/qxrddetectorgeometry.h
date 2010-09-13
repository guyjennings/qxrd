/******************************************************************
*
*  $Id: qxrddetectorgeometry.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

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
  double convertEnergyToWavelength(double energy);
  double convertWavelengthToEnergy(double wavelength);
  double convertTwoThetaToQ(double twoTheta,double wavelength);
  double convertQToTwoTheta(double Q, double wavelength);

  double getTwoTheta(double xCenter,double yCenter,
                   double distance,double xPixel,double yPixel,
                   double pixelLength,double pixelHeight,
                   double cos_beta,double sin_beta,
                   double cos_rotation,double sin_rotation);

  void getTwoThetaChi(double xCenter,double yCenter,
                      double distance,double xPixel,double yPixel,
                      double pixelLength,double pixelHeight,
                      double rotation,double cos_beta,double sin_beta,
                      double cos_alpha,double sin_alpha,
                      double cos_rotation,double sin_rotation,
                      double *twoTheta,double *chi);

  void getQChi(double xCenter,double yCenter,double distance,
               double energy,double xPixel,double yPixel,
               double pixelLength,double pixelHeight,
               double rotation,double cos_beta,double sin_beta,
               double cos_alpha,double sin_alpha,
               double cos_rotation,double sin_rotation,
               double *q,double *chi);

  void getXY(double xCenter,double yCenter,double distance,
             double energy,double q,double chi,double pixelLength,
             double pixelHeight,double rotation,double cos_beta,
             double sin_beta,double cos_alpha,double sin_alpha,
             double cos_rotation,double sin_rotation,
             double * xPixel,double * yPixel);

private:
  HEADER_IDENT("$Id: qxrddetectorgeometry.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDDETECTORGEOMETRY_H

/******************************************************************
*
*  $Log: qxrddetectorgeometry.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/09/10 18:54:10  jennings
*  Partial implement tilt in center finder dialog
*
*  Revision 1.1.2.2  2010/09/09 21:36:31  jennings
*  Made QxrdCenterFinder descend from QxrdDetectorGeometry
*
*  Revision 1.1.2.1  2010/07/27 21:53:03  jennings
*  Added double list property type
*  Added 'testImage' script object, interface to QxrdGenerateTestImage
*  Added QxrdDetectorGeometry class
*
*
*******************************************************************/

