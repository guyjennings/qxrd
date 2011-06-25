#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include "qcepmacros.h"

#include <QObject>
#include <QPen>
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include <qwt_double_rect.h>
#include "qxrddetectorgeometry.h"

class QxrdCenterFinder : public QxrdDetectorGeometry
{
  Q_OBJECT;

public:
  QxrdCenterFinder(QObject *parent=0);

public:
  Q_PROPERTY(double centerX READ get_CenterX WRITE set_CenterX);
  QCEP_DOUBLE_PROPERTY(CenterX);

  Q_PROPERTY(double centerY READ get_CenterY WRITE set_CenterY);
  QCEP_DOUBLE_PROPERTY(CenterY);

  Q_PROPERTY(double centerStep READ get_CenterStep WRITE set_CenterStep);
  QCEP_DOUBLE_PROPERTY(CenterStep);

  Q_PROPERTY(double detectorXPixelSize READ get_DetectorXPixelSize WRITE set_DetectorXPixelSize);
  QCEP_DOUBLE_PROPERTY(DetectorXPixelSize);

  Q_PROPERTY(double detectorYPixelSize READ get_DetectorYPixelSize WRITE set_DetectorYPixelSize);
  QCEP_DOUBLE_PROPERTY(DetectorYPixelSize);

  Q_PROPERTY(double detectorDistance READ get_DetectorDistance WRITE set_DetectorDistance);
  QCEP_DOUBLE_PROPERTY(DetectorDistance);

  Q_PROPERTY(double energy READ get_Energy WRITE set_Energy);
  QCEP_DOUBLE_PROPERTY(Energy);

  Q_PROPERTY(bool    implementTilt    READ get_ImplementTilt WRITE set_ImplementTilt);
  QCEP_BOOLEAN_PROPERTY(ImplementTilt);

  Q_PROPERTY(double detectorTilt READ get_DetectorTilt WRITE set_DetectorTilt);
  QCEP_DOUBLE_PROPERTY(DetectorTilt);

  Q_PROPERTY(double tiltPlaneRotation READ get_TiltPlaneRotation WRITE set_TiltPlaneRotation);
  QCEP_DOUBLE_PROPERTY(TiltPlaneRotation);

//public slots:
//  void onCenterXChanged(double cx);
//  void onCenterYChanged(double cy);
//  void onCenterChanged(QwtDoublePoint pt);
//  void onCenterChanged(QwtDoublePoint pt);
//  void onCenterStepChanged(double stp);

signals:
  void parameterChanged();

public slots:
  void onCenterChanged(QwtDoublePoint pt);

  double getTTH(double x, double y) const;
  double getTTH(QwtDoublePoint pt) const;

  double getQ(double x, double y) const;
  double getQ(QwtDoublePoint pt) const;

  double getR(double x, double y) const;
  double getR(QwtDoublePoint pt) const;

public:
//  void setEnabled(bool imgenabled, bool cntrenabled);
//  void setPen(const QPen &pen);

  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

//signals:
////  void centerChanged(double cx, double cy);
//
//private:
////
//
private:
  mutable QMutex             m_Mutex;
};

#endif // QXRDCENTERFINDER_H
