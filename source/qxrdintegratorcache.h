#ifndef QXRDINTEGRATORCACHE_H
#define QXRDINTEGRATORCACHE_H

#include "qcepmacros.h"

#include <QObject>
#include "qcepproperty.h"

#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qcepintegrateddata-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdpolartransform-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdscriptengine-ptr.h"
#include <QScriptValue>

class QxrdIntegratorCache : public QObject
{
  Q_OBJECT
public:
  QxrdIntegratorCache(QxrdExperimentWPtr exp,
                      QxrdIntegratorWPtr integ,
                      QxrdPolarTransformWPtr xform,
                      QxrdCenterFinderWPtr cf);
  virtual ~QxrdIntegratorCache();

  int get_NRows() const;
  int get_NCols() const;

  void grabScriptEngine();
  void releaseScriptEngine();

  QcepUInt32ImageDataPtr  cachedGeometry();
  QcepUInt32ImageDataPtr  cachedPolarGeometry();
  QcepDoubleImageDataPtr cachedIntensity();

private:
  QMutex m_Mutex;
  int    m_ThreadCount;
  int    m_Oversample;
  double m_RadialStep;
  double m_RadialNSteps;
  double m_RadialStart;
  double m_RadialEnd;
  int    m_RadialUnits;
  double m_PolarStep;
  double m_PolarNSteps;
  double m_PolarStart;
  double m_PolarEnd;
  int    m_PolarUnits;
  double m_CenterX;
  double m_CenterY;
  double m_DetectorXPixelSize;
  double m_DetectorYPixelSize;
  double m_DetectorDistance;
  double m_Energy;
  bool   m_ImplementTilt;
  double m_DetectorTilt;
  double m_TiltPlaneRotation;
  bool   m_EnableGeometry;
  bool   m_EnablePolarization;
  double m_Polarization;
  bool   m_EnableAbsorption;
  double m_AttenuationLength;
  int    m_NRows;
  int    m_NCols;
  int    m_NPix;
  double m_RStep;
  bool   m_RFirst;
  double m_RMin;
  double m_RMax;
  double m_CStep;
  bool   m_CFirst;
  double m_CMin;
  double m_CMax;

  int    m_ResultSize;
  int    m_NRSteps;
  int    m_NCSteps;

  double m_Beta;
  double m_CosBeta;
  double m_SinBeta;
  double m_Rot;
  double m_CosRot;
  double m_SinRot;
  int    m_EnableUserGeometry;
  QString m_UserGeometryScript;
  QString m_UserGeometryFunction;
  int    m_EnableUserAbsorption;
  QString m_UserAbsorptionScript;
  QString m_UserAbsorptionFunction;
  double m_ScalingFactor;
  bool   m_SelfNormalization;
  double m_SelfNormalizationMinimum;
  double m_SelfNormalizationMaximum;

public:
    QcepDataObjectPtr performIntegration(QcepDoubleImageDataPtr dimg,
                                             QcepMaskDataPtr mask,
                                             int normalize);

private:
  double getTTH(double x, double y);
  double getDistance(double x, double y);
  double getChi(double x, double y);
  double getQ(double x, double y);
  double getR(double x, double y);
  double XValue(double x, double y);
  double YValue(double x, double y);
  double NormValue(double x, double y);
  QString XLabel() const;
  QString XUnits() const;
  QString YLabel() const;
  QString YUnits() const;

  void partialIntegrationStep1(int i, int n);
  void partialIntegrationStep2(int i, int n);
  void partialIntegrationStep3(int i, int n, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask, int normalize);

private:
  QAtomicInt             m_CacheFillLevel;
  QAtomicInt             m_CacheFullLevel;
  bool                   m_HasChi;
  QcepUInt32ImageDataPtr  m_CachedRadialBinNumbers;
  QcepUInt32ImageDataPtr  m_CachedPolarBinNumbers;
  QcepDoubleImageDataPtr m_CachedNormalization;
  QcepDoubleImageDataPtr m_CachedRadialValues;
  QcepDoubleImageDataPtr m_CachedPolarValues;
  QVector<double>        m_Integral;
  QVector<double>        m_SumValue;
  QxrdExperimentWPtr     m_Experiment;
  QxrdIntegratorWPtr     m_Integrator;
  QxrdPolarTransformWPtr m_PolarTransform;
  QxrdCenterFinderWPtr   m_CenterFinder;
  QScriptValue           m_UserGeometryFunctionValue;
  QScriptValue           m_UserAbsorptionFunctionValue;
};

#endif // QXRDINTEGRATORCACHE_H
