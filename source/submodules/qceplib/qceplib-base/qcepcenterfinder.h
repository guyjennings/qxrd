#ifndef QCEPCENTERFINDER_H
#define QCEPCENTERFINDER_H

#include "qceplib_global.h"
#include "qcepmacros.h"

#include <QObject>
#include <QPen>
#include "qcepproperty.h"
#include "qcepdetectorgeometry.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagedata.h"
#include <QScriptValue>
#include "qceppowderpointproperty.h"
#include "qcepcenterfinder-ptr.h"
#include "qcepfittedrings-ptr.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepcalibrantdspacings-ptr.h"

class QCEP_EXPORT QcepCenterFinder : public QcepDetectorGeometry
{
  Q_OBJECT

private:
  typedef QcepDetectorGeometry inherited;

public:
  Q_INVOKABLE QcepCenterFinder(QString name);
  void initialize(QcepObjectWPtr parent);

  void setDataset(QcepDatasetModelWPtr dataset);
  void setCalibrantDSpacings(QcepCalibrantDSpacingsWPtr dspacings);

  static QcepCenterFinderWPtr findCenterFinder(QcepObjectWPtr p);

  virtual ~QcepCenterFinder();

public:
  Q_PROPERTY(QPointF center READ get_Center WRITE set_Center)
  QCEP_DOUBLE_POINT_PROPERTY(Center)

//  Q_PROPERTY(double centerX READ get_CenterX WRITE set_CenterX)
//  QCEP_DOUBLE_PROPERTY(CenterX)

//  Q_PROPERTY(double centerY READ get_CenterY WRITE set_CenterY)
//  QCEP_DOUBLE_PROPERTY(CenterY)

  Q_PROPERTY(double centerStep READ get_CenterStep WRITE set_CenterStep)
  QCEP_DOUBLE_PROPERTY(CenterStep)

  Q_PROPERTY(double detectorXPixelSize READ get_DetectorXPixelSize WRITE set_DetectorXPixelSize)
  QCEP_DOUBLE_PROPERTY(DetectorXPixelSize)

  Q_PROPERTY(double detectorYPixelSize READ get_DetectorYPixelSize WRITE set_DetectorYPixelSize)
  QCEP_DOUBLE_PROPERTY(DetectorYPixelSize)

  Q_PROPERTY(double detectorDistance READ get_DetectorDistance WRITE set_DetectorDistance)
  QCEP_DOUBLE_PROPERTY(DetectorDistance)

  Q_PROPERTY(double detectorDistanceStep READ get_DetectorDistanceStep WRITE set_DetectorDistanceStep)
  QCEP_DOUBLE_PROPERTY(DetectorDistanceStep)

  Q_PROPERTY(double energy READ get_Energy WRITE set_Energy)
  QCEP_DOUBLE_PROPERTY(Energy)

  Q_PROPERTY(bool    implementTilt    READ get_ImplementTilt WRITE set_ImplementTilt)
  QCEP_BOOLEAN_PROPERTY(ImplementTilt)

  Q_PROPERTY(double detectorTilt READ get_DetectorTilt WRITE set_DetectorTilt)
  QCEP_DOUBLE_PROPERTY(DetectorTilt)

  Q_PROPERTY(double detectorTiltStep READ get_DetectorTiltStep WRITE set_DetectorTiltStep)
  QCEP_DOUBLE_PROPERTY(DetectorTiltStep)

  Q_PROPERTY(double tiltPlaneRotation READ get_TiltPlaneRotation WRITE set_TiltPlaneRotation)
  QCEP_DOUBLE_PROPERTY(TiltPlaneRotation)

  Q_PROPERTY(double tiltPlaneRotationStep READ get_TiltPlaneRotationStep WRITE set_TiltPlaneRotationStep)
  QCEP_DOUBLE_PROPERTY(TiltPlaneRotationStep)

  Q_PROPERTY(QcepPowderPointVector markedPoints READ get_MarkedPoints WRITE set_MarkedPoints)
  QCEP_POWDERPOINTVECTOR_PROPERTY(MarkedPoints)

  Q_PROPERTY(QcepPowderPointVector fittedRings READ get_FittedRings WRITE set_FittedRings)
  QCEP_POWDERPOINTVECTOR_PROPERTY(FittedRings)

  Q_PROPERTY(double ringRadius READ get_RingRadius WRITE set_RingRadius)
  QCEP_DOUBLE_PROPERTY(RingRadius)

  Q_PROPERTY(double ringRadiusA READ get_RingRadiusA WRITE set_RingRadiusA)
  QCEP_DOUBLE_PROPERTY(RingRadiusA)

  Q_PROPERTY(double ringRadiusB READ get_RingRadiusB WRITE set_RingRadiusB)
  QCEP_DOUBLE_PROPERTY(RingRadiusB)

  Q_PROPERTY(double ringRotation READ get_RingRotation WRITE set_RingRotation)
  QCEP_DOUBLE_PROPERTY(RingRotation)

  Q_PROPERTY(double peakFitRadius READ get_PeakFitRadius WRITE set_PeakFitRadius)
  QCEP_DOUBLE_PROPERTY(PeakFitRadius)

  Q_PROPERTY(double peakHeight READ get_PeakHeight WRITE set_PeakHeight)
  QCEP_DOUBLE_PROPERTY(PeakHeight)

  Q_PROPERTY(double peakCenterX READ get_PeakCenterX WRITE set_PeakCenterX)
  QCEP_DOUBLE_PROPERTY(PeakCenterX)

  Q_PROPERTY(double peakCenterY READ get_PeakCenterY WRITE set_PeakCenterY)
  QCEP_DOUBLE_PROPERTY(PeakCenterY)

  Q_PROPERTY(double peakWidth READ get_PeakWidth WRITE set_PeakWidth)
  QCEP_DOUBLE_PROPERTY(PeakWidth)

  Q_PROPERTY(double peakBackground READ get_PeakBackground WRITE set_PeakBackground)
  QCEP_DOUBLE_PROPERTY(PeakBackground)

  Q_PROPERTY(double peakBackgroundX READ get_PeakBackgroundX WRITE set_PeakBackgroundX)
  QCEP_DOUBLE_PROPERTY(PeakBackgroundX)

  Q_PROPERTY(double peakBackgroundY READ get_PeakBackgroundY WRITE set_PeakBackgroundY)
  QCEP_DOUBLE_PROPERTY(PeakBackgroundY)

  Q_PROPERTY(bool peakFitDebug READ get_PeakFitDebug WRITE set_PeakFitDebug)
  QCEP_BOOLEAN_PROPERTY(PeakFitDebug)

  Q_PROPERTY(int peakFitIterations READ get_PeakFitIterations WRITE set_PeakFitIterations)
  QCEP_INTEGER_PROPERTY(PeakFitIterations)

  Q_PROPERTY(QcepDoubleVector ringAngles READ get_RingAngles WRITE set_RingAngles)
  QCEP_DOUBLE_VECTOR_PROPERTY(RingAngles)

  Q_PROPERTY(double ringAngleTolerance READ get_RingAngleTolerance WRITE set_RingAngleTolerance)
  QCEP_DOUBLE_PROPERTY(RingAngleTolerance)

  Q_PROPERTY(int powderFitOptions READ get_PowderFitOptions WRITE set_PowderFitOptions)
  QCEP_INTEGER_PROPERTY(PowderFitOptions)

  Q_PROPERTY(int ringIndex READ get_RingIndex WRITE set_RingIndex)
  QCEP_INTEGER_PROPERTY(RingIndex)

  Q_PROPERTY(bool subtractRingAverages READ get_SubtractRingAverages WRITE set_SubtractRingAverages)
  QCEP_BOOLEAN_PROPERTY(SubtractRingAverages)

  Q_PROPERTY(double ringAverageDisplacement READ get_RingAverageDisplacement WRITE set_RingAverageDisplacement)
  QCEP_DOUBLE_PROPERTY(RingAverageDisplacement)

  Q_PROPERTY(double fittedWidthMin READ get_FittedWidthMin WRITE set_FittedWidthMin)
  QCEP_DOUBLE_PROPERTY(FittedWidthMin)

  Q_PROPERTY(double fittedWidthMax READ get_FittedWidthMax WRITE set_FittedWidthMax)
  QCEP_DOUBLE_PROPERTY(FittedWidthMax)

  Q_PROPERTY(double fittedHeightMinRatio READ get_FittedHeightMinRatio WRITE set_FittedHeightMinRatio)
  QCEP_DOUBLE_PROPERTY(FittedHeightMinRatio)

  Q_PROPERTY(double fittedPositionMaxDistance READ get_FittedPositionMaxDistance WRITE set_FittedPositionMaxDistance)
  QCEP_DOUBLE_PROPERTY(FittedPositionMaxDistance)

  Q_PROPERTY(bool fitPowderPointPosition READ get_FitPowderPointPosition WRITE set_FitPowderPointPosition)
  QCEP_BOOLEAN_PROPERTY(FitPowderPointPosition)

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
  void centerMoveUpLeft();
  void centerMoveUp();
  void centerMoveUpRight();
  void centerMoveRight();
  void centerMoveDownRight();
  void centerMoveDown();
  void centerMoveDownLeft();
  void centerMoveLeft();

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

  QPointF getXY(double tth, double chi);

  void fitPowderCircle(int n=0);
  void fitPowderEllipse(int n=0);
  void fitPowderEllipses();

  void adjustEnergy(int n);
  void adjustDistance(int n);

  void deletePowderPointNear(double x, double y);
  void deletePowderPoints();
  void deletePowderRing(int n);
  void disablePowderRing(int n);
  void enablePowderRing(int n);
  void appendPowderPoint(double x, double y);
  void appendPowderPoint(int n1, int n2, int n3, double x, double y, double r1=0, double r2=0, double az=0);
  void normalizePowderRings();

  QcepPowderPoint powderPoint(int i);
  int nearestPowderPointIndex(double x, double y);
  QcepPowderPoint nearestPowderPoint(double x, double y);

  bool fitPeakNear(double x, double y);
  bool fitRingNear(double x0, double y0);
  bool traceRingNear(double x0, double y0, double step=25.0);
//  bool traceRingNearParallel(double x0, double y0, double step=25.0);
  bool missingRingNear(double x, double y);

  int    getPowderPointN1(int i);
  int    getPowderPointN2(int i);
  double getPowderPointX(int i);
  double getPowderPointY(int i);
  void   setPowderPoint(int i, int n1, int n2, int n3, double x, double y, double r1, double r2, double az);

//  QScriptValue getPowderPoint(int i);
//  QScriptValue getPowderPoints();
//  void         setPowderPoint(int i, QScriptValue val);

  int countPowderRings() const;
  int countPowderRingPoints() const;
  int countPowderRingPoints(int r) const;
  QcepPowderPoint powderRingPoint(int i) const;
  QcepPowderPoint powderRingPoint(int r, int i) const;

  double powderRingAverageR(int r) const;
  double powderRingAverageTTH(int r) const;
  double powderRingAverageQ(int r) const;

//  void updateCalibrantDSpacings();
  double calibrantDSpacing(int n);
  double calibrantTTH(int n);

  void calculateCalibration();

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepCenterFinderPtr &proc);
  static void fromScriptValue(const QScriptValue &obj, QcepCenterFinderPtr &proc);

  void setData(QcepDoubleImageDataPtr data);
  double imageValue(double x, double y);

  QcepDoubleImageDataPtr data() const;
  QcepDoubleImageDataPtr newData();

  static QString levmarFailureReason(int n);

  void setFittedRings(QcepFittedRingsPtr rings);

private:
  void moveCenter(int dx, int dy);

private:
  mutable QMutex             m_Mutex;
  QcepDoubleImageDataPtr     m_Data;
  QcepDatasetModelWPtr       m_Dataset;
  QcepCalibrantDSpacingsWPtr m_CalibrantDSpacings;
  int                        m_CenterFitRingNumber;
};

Q_DECLARE_METATYPE(QcepCenterFinder*)

#endif // QCEPCENTERFINDER_H
