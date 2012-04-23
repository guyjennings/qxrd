#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include "qcepmacros.h"

#include <QObject>
#include <QPen>
#include "qcepproperty.h"
#include <qwt_double_rect.h>
#include "qxrddetectorgeometry.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdobjectnamer.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdCenterFinder : public QxrdDetectorGeometry
{
  Q_OBJECT

public:
  QxrdCenterFinder(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr expt);

private:
  QxrdObjectNamer       m_ObjectNamer;

public:
  Q_PROPERTY(double centerX READ get_CenterX WRITE set_CenterX)
  QCEP_DOUBLE_PROPERTY(CenterX)

  Q_PROPERTY(double centerY READ get_CenterY WRITE set_CenterY)
  QCEP_DOUBLE_PROPERTY(CenterY)

  Q_PROPERTY(double centerStep READ get_CenterStep WRITE set_CenterStep)
  QCEP_DOUBLE_PROPERTY(CenterStep)

  Q_PROPERTY(double detectorXPixelSize READ get_DetectorXPixelSize WRITE set_DetectorXPixelSize)
  QCEP_DOUBLE_PROPERTY(DetectorXPixelSize)

  Q_PROPERTY(double detectorYPixelSize READ get_DetectorYPixelSize WRITE set_DetectorYPixelSize)
  QCEP_DOUBLE_PROPERTY(DetectorYPixelSize)

  Q_PROPERTY(double detectorDistance READ get_DetectorDistance WRITE set_DetectorDistance)
  QCEP_DOUBLE_PROPERTY(DetectorDistance)

  Q_PROPERTY(double energy READ get_Energy WRITE set_Energy)
  QCEP_DOUBLE_PROPERTY(Energy)

  Q_PROPERTY(bool    implementTilt    READ get_ImplementTilt WRITE set_ImplementTilt)
  QCEP_BOOLEAN_PROPERTY(ImplementTilt)

  Q_PROPERTY(double detectorTilt READ get_DetectorTilt WRITE set_DetectorTilt)
  QCEP_DOUBLE_PROPERTY(DetectorTilt)

  Q_PROPERTY(double tiltPlaneRotation READ get_TiltPlaneRotation WRITE set_TiltPlaneRotation)
  QCEP_DOUBLE_PROPERTY(TiltPlaneRotation)

  Q_PROPERTY(bool   enableGeometricCorrections READ get_EnableGeometricCorrections WRITE set_EnableGeometricCorrections)
  QCEP_BOOLEAN_PROPERTY(EnableGeometricCorrections)

  Q_PROPERTY(bool   enablePolarizationCorrections READ get_EnablePolarizationCorrections WRITE set_EnablePolarizationCorrections)
  QCEP_BOOLEAN_PROPERTY(EnablePolarizationCorrections)

  Q_PROPERTY(double polarization READ get_Polarization WRITE set_Polarization)
  QCEP_DOUBLE_PROPERTY(Polarization)

  Q_PROPERTY(bool   enableAbsorptionCorrections READ get_EnableAbsorptionCorrections WRITE set_EnableAbsorptionCorrections)
  QCEP_BOOLEAN_PROPERTY(EnableAbsorptionCorrections)

  Q_PROPERTY(double attenuationLength READ get_AttenuationLength WRITE set_AttenuationLength)
  QCEP_DOUBLE_PROPERTY(AttenuationLength)

  Q_PROPERTY(QcepPolygon markedPoints READ get_MarkedPoints WRITE set_MarkedPoints)
  QCEP_POLYGON_PROPERTY(MarkedPoints)

  Q_PROPERTY(double ringRadius READ get_RingRadius WRITE set_RingRadius)
  QCEP_DOUBLE_PROPERTY(RingRadius)

  Q_PROPERTY(bool adjustMarkedPoints READ get_AdjustMarkedPoints WRITE set_AdjustMarkedPoints)
  QCEP_BOOLEAN_PROPERTY(AdjustMarkedPoints)

  Q_PROPERTY(double adjustmentRadius READ get_AdjustmentRadius WRITE set_AdjustmentRadius)
  QCEP_DOUBLE_PROPERTY(AdjustmentRadius)

//public slots:
//  void onCenterXChanged(double cx);
//  void onCenterYChanged(double cy);
//  void onCenterChanged(QwtDoublePoint pt);
//  void onCenterChanged(QwtDoublePoint pt);
//  void onCenterStepChanged(double stp);

signals:
  void parameterChanged();

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  void onCenterChanged(QwtDoublePoint pt);
  void onPointSelected(QwtDoublePoint pt);

  double getTTH(double x, double y) const;
  double getTTH(QwtDoublePoint pt) const;

  double getQ(double x, double y) const;
  double getQ(QwtDoublePoint pt) const;

  double getR(double x, double y) const;
  double getR(QwtDoublePoint pt) const;

  double getChi(double x, double y) const;
  double getChi(QwtDoublePoint pt) const;

  double getDist(double x, double y) const;
  double getDist(QwtDoublePoint pt) const;

  void fitPowderCircle();
  void deletePowderPointNear(double x, double y);
  void deletePowderPoints();

  QwtDoublePoint powderPoint(int i);
  int nearestPowderPointIndex(double x, double y);
  QwtDoublePoint nearestPowderPoint(double x, double y);

  QwtDoublePoint adjustPoint(QwtDoublePoint pt);
  QwtDoublePoint adjustPoint(int i);
  void adjustPointNear(double x, double y);
  void adjustAllPoints();

public:
//  void setEnabled(bool imgenabled, bool cntrenabled);
//  void setPen(const QPen &pen);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void setData(QxrdDoubleImageDataPtr data);
  double imageValue(double x, double y);

  void evaluateFit(double *parm, double *x, int np, int nx);

//signals:
////  void centerChanged(double cx, double cy);
//
//private:
////
//
private:
  mutable QMutex             m_Mutex;
  QxrdExperimentWPtr         m_Experiment;
  QxrdDoubleImageDataPtr     m_Data;
};

#endif // QXRDCENTERFINDER_H
