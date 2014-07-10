#ifndef QXRDCENTERFINDER_H
#define QXRDCENTERFINDER_H

#include "qcepmacros.h"

#include <QObject>
#include <QPen>
#include "qcepproperty.h"
#include "qxrddetectorgeometry.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdobjectnamer.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdimagedata.h"
#include "qxrdexperiment-ptr.h"
#include <QScriptValue>

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

  Q_PROPERTY(int    enableUserGeometry READ get_EnableUserGeometry WRITE set_EnableUserGeometry)
  QCEP_INTEGER_PROPERTY(EnableUserGeometry)

  Q_PROPERTY(QString userGeometryScript READ get_UserGeometryScript WRITE set_UserGeometryScript)
  QCEP_STRING_PROPERTY(UserGeometryScript)

  Q_PROPERTY(QString userGeometryFunction READ get_UserGeometryFunction WRITE set_UserGeometryFunction)
  QCEP_STRING_PROPERTY(UserGeometryFunction)

  Q_PROPERTY(int     enableUserAbsorption READ get_EnableUserAbsorption WRITE set_EnableUserAbsorption)
  QCEP_INTEGER_PROPERTY(EnableUserAbsorption)

  Q_PROPERTY(QString userAbsorptionScript READ get_UserAbsorptionScript WRITE set_UserAbsorptionScript)
  QCEP_STRING_PROPERTY(UserAbsorptionScript)

  Q_PROPERTY(QString userAbsorptionFunction READ get_UserAbsorptionFunction WRITE set_UserAbsorptionFunction)
  QCEP_STRING_PROPERTY(UserAbsorptionFunction)

  Q_PROPERTY(double peakRadius READ get_PeakRadius WRITE set_PeakRadius)
  QCEP_DOUBLE_PROPERTY(PeakRadius)

  Q_PROPERTY(double peakFitRadius READ get_PeakFitRadius WRITE set_PeakFitRadius)
  QCEP_DOUBLE_PROPERTY(PeakFitRadius)

  Q_PROPERTY(double peakHeight READ get_PeakHeight WRITE set_PeakHeight)
  QCEP_DOUBLE_PROPERTY(PeakHeight)

  Q_PROPERTY(double peakCenterX READ get_PeakCenterX WRITE set_PeakCenterX)
  QCEP_DOUBLE_PROPERTY(PeakCenterX)

  Q_PROPERTY(double peakCenterY READ get_PeakCenterY WRITE set_PeakCenterY)
  QCEP_DOUBLE_PROPERTY(PeakCenterY)

  Q_PROPERTY(double peakBackground READ get_PeakBackground WRITE set_PeakBackground)
  QCEP_DOUBLE_PROPERTY(PeakBackground)

  Q_PROPERTY(double peakBackgroundX READ get_PeakBackgroundX WRITE set_PeakBackgroundX)
  QCEP_DOUBLE_PROPERTY(PeakBackgroundX)

  Q_PROPERTY(double peakBackgroundY READ get_PeakBackgroundY WRITE set_PeakBackgroundY)
  QCEP_DOUBLE_PROPERTY(PeakBackgroundY)

  Q_PROPERTY(double peakAzimuth READ get_PeakAzimuth WRITE set_PeakAzimuth)
  QCEP_DOUBLE_PROPERTY(PeakAzimuth)

  Q_PROPERTY(double peakPixelRadius READ get_PeakPixelRadius WRITE set_PeakPixelRadius)
  QCEP_DOUBLE_PROPERTY(PeakPixelRadius)

  Q_PROPERTY(int peakFitDebug READ get_PeakFitDebug WRITE set_PeakFitDebug)
  QCEP_INTEGER_PROPERTY(PeakFitDebug)

public:
  enum {
    UserAbsorptionNone = 0,
    UserAbsorptionPixelCoords = 1,
    UserAbsorptionFromCenter = 2,
    UserAbsorptionRChi = 3,
    UserAbsorptionQChi = 4
  }  UserAbsorptionKinds;

//public slots:
//  void onCenterXChanged(double cx);
//  void onCenterYChanged(double cy);
//  void onCenterChanged(QPointF pt);
//  void onCenterChanged(QPointF pt);
//  void onCenterStepChanged(double stp);

signals:
  void parameterChanged();

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  void onCenterChanged(QPointF pt);
  void onPointSelected(QPointF pt);

  double getTTH(double x, double y) const;
  double getTTH(QPointF pt) const;

  double getQ(double x, double y) const;
  double getQ(QPointF pt) const;

  double getR(double x, double y) const;
  double getR(QPointF pt) const;

  double getChi(double x, double y) const;
  double getChi(QPointF pt) const;

  double getDist(double x, double y) const;
  double getDist(QPointF pt) const;

  void fitPowderCircle();
  void deletePowderPointNear(double x, double y);
  void deletePowderPoints();
  void appendPowderPoint(double x, double y);

  QPointF powderPoint(int i);
  int nearestPowderPointIndex(double x, double y);
  QPointF nearestPowderPoint(double x, double y);

  QPointF adjustPoint(QPointF pt);
  QPointF adjustPoint(int i);
  void adjustPointNear(double x, double y);
  void adjustAllPoints();
  bool fitPeakNear(double x, double y, int nitermax=200);
  bool fitPeakNear();

  bool fitRingNear(double x, double y);
  bool fitRingNear(double x, double y, double step, int nitermax=200);
  bool traceRingNear(double x0, double y0, double step=25.0, int nitermax=200);

  QString defaultUserGeometryScript();
  QString defaultUserAbsorptionScript();

  double getPowderPointX(int i);
  double getPowderPointY(int i);
  void   setPowderPoint(int i, double x, double y);

  QScriptValue getPowderPoint(int i);
  QScriptValue getPowderPoints();
  int          countPowderPoints();
  void         setPowderPoint(int i, QScriptValue val);

public:
//  void setEnabled(bool imgenabled, bool cntrenabled);
//  void setPen(const QPen &pen);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  void setData(QxrdDoubleImageDataPtr data);
  double imageValue(double x, double y);

  void evaluateFit(double *parm, double *x, int np, int nx);
  void evaluatePeakFit(double *parm, double *x, int np, int nx);
  void evaluateRingFit(double *parm, double *x, int np, int nx);

  QxrdExperimentWPtr experiment() const;

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
