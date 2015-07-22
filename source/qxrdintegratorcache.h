#ifndef QXRDINTEGRATORCACHE_H
#define QXRDINTEGRATORCACHE_H

#include "qcepmacros.h"

#include <QObject>
#include "qcepproperty.h"

#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qcepintegrateddata-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdscriptengine-ptr.h"
#include <QScriptValue>

class QxrdIntegratorCache : public QObject
{
  Q_OBJECT
public:
  QxrdIntegratorCache(QxrdExperimentWPtr exp,
                      QxrdIntegratorWPtr integ,
                      QxrdCenterFinderWPtr cf);
  virtual ~QxrdIntegratorCache();

  int get_NRows() const;
  int get_NCols() const;

  void grabScriptEngine();
  void releaseScriptEngine();

  QcepInt32ImageDataPtr  cachedGeometry();
  QcepDoubleImageDataPtr cachedIntensity();

private:
  int    m_Oversample;
  double m_IntegrationStep;
  double m_IntegrationNSteps;
  double m_IntegrationMinimum;
  double m_IntegrationMaximum;
  int    m_IntegrationXUnits;
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
  double m_RMin;
  double m_RMax;
  int    m_NRange;
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
    QcepIntegratedDataPtr performIntegration(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask, int normalize);

private:
  double getTTH(double x, double y);
  double getDistance(double x, double y);
  double getChi(double x, double y);
  double getQ(double x, double y);
  double getR(double x, double y);
  double XValue(double x, double y);
  double NormValue(double x, double y);
  QString XLabel() const;

private:
  QAtomicInt             m_CacheFillLevel;
  QAtomicInt             m_CacheFullLevel;
  QcepInt32ImageDataPtr  m_CachedBinNumbers;
  QcepDoubleImageDataPtr m_CachedNormalization;
  QxrdExperimentWPtr     m_Experiment;
  QxrdIntegratorWPtr     m_Integrator;
  QxrdCenterFinderWPtr   m_CenterFinder;
  QScriptValue           m_UserGeometryFunctionValue;
  QScriptValue           m_UserAbsorptionFunctionValue;
};

#endif // QXRDINTEGRATORCACHE_H
