#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include <QObject>
#include "qxrdintegrateddata.h"
#include "qcepproperty.h"
#include "qxrddataprocessor.h"
#include <qwt_double_rect.h>
#include "qxrdintegratorcache.h"

class QxrdDataProcessorBase;
class QxrdAllocator;
class QxrdExperiment;

class QxrdIntegrator : public QObject
{
  Q_OBJECT

public:
  QxrdIntegrator(QxrdSettingsSaver *saver, QxrdExperiment *exp, QxrdDataProcessorBase *proc, QxrdAllocator *alloc, QObject *parent=0);

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

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);
  QxrdDataProcessorBase *dataProcessor() const;

  enum {
    IntegrateTTH, IntegrateQ, IntegrateR
  };

  double XValue(double x, double y) const;
  double XValue(QwtDoublePoint pt) const;
  double XValue(double x, double y,
                int xUnits, QxrdCenterFinder *cf,
                double xc, double yc,
                double dst, double nrg,
                double pxl, double pxh,
                double rot, double cosr, double sinr,
                double cosb, double sinb,
                double cosa, double sina
                ) const;

  QString XLabel() const;

public slots:
  QxrdIntegratedDataPtr performIntegration(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask);
//  QxrdIntegratedDataPtr integrate(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, int oversample, int normalize);

  QxrdIntegratedDataPtr sliceLine(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, double x0, double y0, double x1, double y1, double width);
  QxrdIntegratedDataPtr slicePolygon(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QwtArray<QwtDoublePoint> poly, double width);

  void integrateVsR();
  void integrateVsQ();
  void integrateVsTTH();

  void onIntegrationParametersChanged();

private:
  mutable QMutex         m_Mutex;
  QxrdExperiment        *m_Experiment;
  QxrdDataProcessorBase *m_DataProcessor;
  QxrdAllocator         *m_Allocator;
  QxrdIntegratorCachePtr m_IntegratorCache;
};

#endif // QXRDINTEGRATOR_H
