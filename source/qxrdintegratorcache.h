#ifndef QXRDINTEGRATORCACHE_H
#define QXRDINTEGRATORCACHE_H

#include "qcepmacros.h"

#include <QObject>
#include "qxrdintegrateddata.h"
#include "qcepproperty.h"
#include "qxrddataprocessor.h"
#include "qxrdallocator.h"

class QxrdExperiment;
typedef QSharedPointer<QxrdExperiment> QxrdExperimentPtr;

class QxrdIntegratorCache : public QObject
{
  Q_OBJECT
public:
  QxrdIntegratorCache(QxrdExperimentPtr exp, QxrdAllocatorPtr alloc);

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

  Q_PROPERTY(int     nRows      READ get_NRows WRITE set_NRows)
  QCEP_INTEGER_PROPERTY(NRows)

  Q_PROPERTY(int     nCols      READ get_NCols WRITE set_NCols)
  QCEP_INTEGER_PROPERTY(NCols)

  Q_PROPERTY(double rStep READ get_RStep WRITE set_RStep)
  QCEP_DOUBLE_PROPERTY(RStep)

  Q_PROPERTY(double rMin READ get_RMin WRITE set_RMin)
  QCEP_DOUBLE_PROPERTY(RMin)

  Q_PROPERTY(double rMax READ get_RMax WRITE set_RMax)
  QCEP_DOUBLE_PROPERTY(RMax)

  Q_PROPERTY(double nMin READ get_NMin WRITE set_NMin)
  QCEP_DOUBLE_PROPERTY(NMin)

  Q_PROPERTY(double nMax READ get_NMax WRITE set_NMax)
  QCEP_DOUBLE_PROPERTY(NMax)

  Q_PROPERTY(int nRange READ get_NRange WRITE set_NRange)
  QCEP_DOUBLE_PROPERTY(NRange)

  Q_PROPERTY(double beta READ get_Beta WRITE set_Beta)
  QCEP_DOUBLE_PROPERTY(Beta)

  Q_PROPERTY(double cosBeta READ get_CosBeta WRITE set_CosBeta)
  QCEP_DOUBLE_PROPERTY(CosBeta)

  Q_PROPERTY(double sinBeta READ get_SinBeta WRITE set_SinBeta)
  QCEP_DOUBLE_PROPERTY(SinBeta)

  Q_PROPERTY(double rot READ get_Rot WRITE set_Rot)
  QCEP_DOUBLE_PROPERTY(Rot)

  Q_PROPERTY(double cosRot READ get_CosRot WRITE set_CosRot)
  QCEP_DOUBLE_PROPERTY(CosRot)

  Q_PROPERTY(double sinRot READ get_SinRot WRITE set_SinRot)
  QCEP_DOUBLE_PROPERTY(SinRot)

public:
    QxrdIntegratedDataPtr performIntegration(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, int normalize);

private:
  double getTTH(double x, double y);
  double getQ(double x, double y);
  double getR(double x, double y);
  double XValue(double x, double y);
  QString XLabel() const;

private:
  QAtomicInt             m_CacheFillLevel;
  QAtomicInt             m_CacheFullLevel;
  QxrdInt32ImageDataPtr  m_CachedBinNumbers;
  QxrdExperimentPtr      m_Experiment;
  QxrdAllocatorPtr       m_Allocator;
};

typedef QSharedPointer<QxrdIntegratorCache> QxrdIntegratorCachePtr;

#endif // QXRDINTEGRATORCACHE_H
