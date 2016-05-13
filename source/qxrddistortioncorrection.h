#ifndef QXRDDISTORTIONCORRECTION_H
#define QXRDDISTORTIONCORRECTION_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdexperiment-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepimagedata.h"
#include "qcepsettingssaver-ptr.h"

class QxrdDistortionCorrection : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdDistortionCorrection(QxrdExperimentWPtr expt);

public:
  Q_PROPERTY(QString distortionImagePath READ get_DistortionImagePath WRITE set_DistortionImagePath)
  QCEP_STRING_PROPERTY(DistortionImagePath)

  Q_PROPERTY(QPointF p0 READ get_P0 WRITE set_P0)
  QCEP_DOUBLE_POINT_PROPERTY(P0)

  Q_PROPERTY(QPointF p1 READ get_P1 WRITE set_P1)
  QCEP_DOUBLE_POINT_PROPERTY(P1)

  Q_PROPERTY(QPointF p2 READ get_P2 WRITE set_P2)
  QCEP_DOUBLE_POINT_PROPERTY(P2)

  Q_PROPERTY(int n1 READ get_N1 WRITE set_N1)
  QCEP_INTEGER_PROPERTY(N1)

  Q_PROPERTY(int n2 READ get_N2 WRITE set_N2)
  QCEP_INTEGER_PROPERTY(N2)

  Q_PROPERTY(QPointF f0 READ get_F0 WRITE set_F0)
  QCEP_DOUBLE_POINT_PROPERTY(F0)

  Q_PROPERTY(QPointF f1 READ get_F1 WRITE set_F1)
  QCEP_DOUBLE_POINT_PROPERTY(F1)

  Q_PROPERTY(QPointF f2 READ get_F2 WRITE set_F2)
  QCEP_DOUBLE_POINT_PROPERTY(F2)

  Q_PROPERTY(QcepIntVector iVals READ get_IVals WRITE set_IVals)
  QCEP_INTEGER_VECTOR_PROPERTY(IVals)

  Q_PROPERTY(QcepIntVector jVals READ get_JVals WRITE set_JVals)
  QCEP_INTEGER_VECTOR_PROPERTY(JVals)

  Q_PROPERTY(QcepDoubleVector xVals READ get_XVals WRITE set_XVals)
  QCEP_DOUBLE_VECTOR_PROPERTY(XVals)

  Q_PROPERTY(QcepDoubleVector yVals READ get_YVals WRITE set_YVals)
  QCEP_DOUBLE_VECTOR_PROPERTY(YVals)

  Q_PROPERTY(QcepDoubleVector fxVals READ get_FXVals WRITE set_FXVals)
  QCEP_DOUBLE_VECTOR_PROPERTY(FXVals)

  Q_PROPERTY(QcepDoubleVector fyVals READ get_FYVals WRITE set_FYVals)
  QCEP_DOUBLE_VECTOR_PROPERTY(FYVals)

  Q_PROPERTY(QcepDoubleVector dxVals READ get_DXVals WRITE set_DXVals)
  QCEP_DOUBLE_VECTOR_PROPERTY(DXVals)

  Q_PROPERTY(QcepDoubleVector dyVals READ get_DYVals WRITE set_DYVals)
  QCEP_DOUBLE_VECTOR_PROPERTY(DYVals)

  Q_PROPERTY(double wMin READ get_WMin WRITE set_WMin)
  QCEP_DOUBLE_PROPERTY(WMin)

  Q_PROPERTY(double wMax READ get_WMax WRITE set_WMax)
  QCEP_DOUBLE_PROPERTY(WMax)

  Q_PROPERTY(double wNom READ get_WNom WRITE set_WNom)
  QCEP_DOUBLE_PROPERTY(WNom)

  Q_PROPERTY(double ratMin READ get_RatMin WRITE set_RatMin)
  QCEP_DOUBLE_PROPERTY(RatMin)

  Q_PROPERTY(double hgtMin READ get_HgtMin WRITE set_HgtMin)
  QCEP_DOUBLE_PROPERTY(HgtMin)

  Q_PROPERTY(QPointF distMax READ get_DistMax WRITE set_DistMax)
  QCEP_DOUBLE_POINT_PROPERTY(DistMax)

  signals:

public slots:
  QPointF transform(QPointF pt);
  QPointF inverse(QPointF pt);

  void clearGridPoints();
  void appendGridPoint(int i, int j, double x, double y);
  void fitCalibrationGrid();
  void evalCalibrationGrid();
  void dumpCalibrationGrid(QString path);

public:
  void evaluateFitGrid(double parms[], double hx[], int m, int n);

private:
  mutable QMutex             m_Mutex;
  QxrdExperimentWPtr         m_Experiment;
  QcepDoubleImageDataPtr     m_Data;
};

Q_DECLARE_METATYPE(QxrdDistortionCorrection*)

#endif // QXRDDISTORTIONCORRECTION_H
