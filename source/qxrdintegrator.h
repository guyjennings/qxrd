#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include "qcepobject.h"
#include "qcepproperty.h"

#include "qxrdexperiment-ptr.h"
#include "qcepintegrateddata.h"
#include "qcepintegrateddata-ptr.h"
#include "qxrdintegratorcache.h"
#include "qxrdintegratorcache-ptr.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qcepallocator-ptr.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata.h"
#include "qcepmaskdata-ptr.h"
#include "qxrddataprocessor-ptr.h"

class QxrdIntegrator : public QcepObject
{
  Q_OBJECT

public:
  QxrdIntegrator(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdCenterFinderWPtr cfw);
  virtual ~QxrdIntegrator();
  void initialize(QxrdIntegratorWPtr integrator);

public:
  Q_PROPERTY(int oversample READ get_Oversample WRITE set_Oversample)
  QCEP_INTEGER_PROPERTY(Oversample)

  Q_PROPERTY(double integrationStep READ get_IntegrationStep WRITE set_IntegrationStep)
  QCEP_DOUBLE_PROPERTY(IntegrationStep)

  Q_PROPERTY(double integrationNSteps READ get_IntegrationNSteps WRITE set_IntegrationNSteps)
  QCEP_INTEGER_PROPERTY(IntegrationNSteps)

  Q_PROPERTY(double integrationMinimum READ get_IntegrationMinimum WRITE set_IntegrationMinimum)
  QCEP_DOUBLE_PROPERTY(IntegrationMinimum)

  Q_PROPERTY(double integrationMaximum READ get_IntegrationMaximum WRITE set_IntegrationMaximum)
  QCEP_DOUBLE_PROPERTY(IntegrationMaximum)

  Q_PROPERTY(int integrationXUnits READ get_IntegrationXUnits WRITE set_IntegrationXUnits)
  QCEP_INTEGER_PROPERTY(IntegrationXUnits)

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

  Q_PROPERTY(double scalingFactor READ get_ScalingFactor WRITE set_ScalingFactor)
  QCEP_DOUBLE_PROPERTY(ScalingFactor)

public:
  QxrdDataProcessorWPtr dataProcessor() const;
  QxrdExperimentWPtr experiment() const;

  enum {
    IntegrateTTH, IntegrateQ, IntegrateR
  };

  double XValue(double x, double y) const;
  double XValue(QPointF pt) const;
  double XValue(double x, double y,
                int xUnits, QxrdCenterFinderPtr cf,
                double xc, double yc,
                double dst, double nrg,
                double pxl, double pxh,
                double rot, double cosr, double sinr,
                double cosb, double sinb,
                double cosa, double sina
                ) const;

  QString XLabel() const;

public slots:
  void                  prepareAccumulator(QString resPath, int nImages);
  void                  completeAccumulator(QString path);

  void                  appendIntegration(QString resPath, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask = QcepMaskDataPtr());
  void                  appendIntegration(QString resPath, QcepIntegratedDataPtr integ);
  void                  appendIntegration(QcepDoubleImageDataPtr res, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask = QcepMaskDataPtr());
  void                  appendIntegration(QcepDoubleImageDataPtr res, QcepIntegratedDataPtr integ);
  QcepIntegratedDataPtr performIntegration(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask);
  QcepIntegratedDataPtr performIntegration(QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask = QcepMaskDataPtr());

//  QcepIntegratedDataPtr integrate(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask, int oversample, int normalize);

  QcepIntegratedDataPtr sliceLine(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr dimg, double x0, double y0, double x1, double y1, double width);
  QcepIntegratedDataPtr slicePolygon(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr dimg, QVector<QPointF> poly, double width);

  void integrateVsR();
  void integrateVsQ();
  void integrateVsTTH();

  void onIntegrationParametersChanged();

  QcepInt32ImageDataPtr  cachedGeometry();
  QcepDoubleImageDataPtr cachedIntensity();

  QString defaultUserGeometryScript();
  QString defaultUserAbsorptionScript();

private:
  mutable QMutex         m_Mutex;
  QcepSettingsSaverWPtr  m_Saver;
  QxrdExperimentWPtr     m_Experiment;
  QxrdIntegratorWPtr     m_Integrator;
  QxrdCenterFinderWPtr   m_CenterFinder;
  QxrdIntegratorCachePtr m_IntegratorCache;
};

typedef QSharedPointer<QxrdIntegrator> QxrdIntegratorPtr;
typedef QWeakPointer<QxrdIntegrator> QxrdIntegratorWPtr;

#endif // QXRDINTEGRATOR_H
