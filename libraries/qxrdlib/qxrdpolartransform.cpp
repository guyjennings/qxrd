#include "qxrdpolartransform.h"
#include "qxrdexperiment.h"
#include "qxrdintegratorcache.h"
#include "qxrddataprocessor.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdatasetmodel.h"
#include "qcepimagedata.h"
#include "qcepallocator.h"

QxrdPolarTransform::QxrdPolarTransform(QString name) :
  QcepObject(name),
//  m_Destination(saver, this, "destination", "Polar/image", "Destination for polar transform"),
//  m_OutputType(saver, this, "outputType", 3, "Output type 0=none, 1=data, 2=radial plot, 3=polar plot"),
  m_Oversample(this, "oversample", 1, "Oversample factor"),
  m_EnableGeometricCorrections(this, "enableGeometricCorrections", false, "Enable Geometric Corrections (tilt and distance) in Integration"),
  m_EnablePolarizationCorrections(this, "enablePolarizationCorrections", false, "Enable Polarization Corrections in Integration"),
  m_Polarization(this, "polarization", 1.0, "Beam Polarization Factor"),
  m_PolarUnits(this, "polarUnits", QxrdIntegrator::PolarIntegrateChi, "Polar Integration Type (0=Angle)"),
  m_PolarStep(this, "polarStep", 0.5, "Polar Integration Step Size"),
  m_PolarNSteps(this, "polarNSteps", 0, "Polar Integration Number of Steps"),
  m_PolarStart(this, "polarStart", 0, "Polar Integration Start"),
  m_PolarEnd(this, "polarEnd", 360, "Polar Integration End"),
  m_RadialUnits(this, "radialUnits", QxrdIntegrator::RadialIntegrateTTH, "Radial Units for Integration (0 = TTH, 1 = Q, 2 = R)"),
  m_RadialStep(this, "radialStep", 0.001, "Radial Integration Step Size"),
  m_RadialNSteps(this, "radialNSteps", 0, "Radial Integration Number of Steps"),
  m_RadialStart(this, "radialStart", 0, "Radial Integration Start"),
  m_RadialEnd(this, "radialEnd", 100000, "Radial Integration End")
{
}

QxrdPolarTransformPtr QxrdPolarTransform::newPolarTransform()
{
  QxrdPolarTransformPtr xform(new QxrdPolarTransform("polarTransform"));

  return xform;
}

QxrdPolarTransform::~QxrdPolarTransform()
{
}

QxrdExperimentWPtr QxrdPolarTransform::experiment() const
{
  QxrdExperimentWPtr expt = qSharedPointerDynamicCast<QxrdExperiment>(parentPtr());

  return expt;
}

QxrdIntegratorWPtr QxrdPolarTransform::integrator() const
{
  QxrdIntegratorWPtr res;

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    res = expt->integrator();
  }

  return res;
}

//void QxrdPolarTransform::execute()
//{
//  QxrdExperimentPtr expt(m_Experiment);

//  if (expt) {
//    QxrdDataProcessorPtr proc(expt->dataProcessor());
//    QxrdCenterFinderPtr  cf(expt->centerFinder());
//    QxrdIntegratorPtr    integ(expt->integrator());

//    if (proc && cf) {
//      QcepDoubleImageDataPtr img  = proc->data();
//      QcepMaskDataPtr        mask = proc->mask();

//      m_IntegratorCache =
//          QxrdIntegratorCachePtr(new QxrdIntegratorCache(
//                                   expt, integ, sharedFromThis(), cf));

//      QcepDatasetModelPtr    ds  = expt->dataset();

//      QcepDoubleImageDataPtr res = ds->image(get_Destination());

//      if (res == NULL) {
//        res = ds->newImage(get_Destination());
//      }

//      m_IntegratorCache->performIntegration(res, img, mask, 0);
//    }
//  }
//}

QcepDataObjectPtr QxrdPolarTransform::transform(QcepDoubleImageDataPtr img, QcepMaskDataPtr mask)
{
  QcepDataObjectPtr res;

  QxrdExperimentPtr expt(experiment());

  if (expt) {
    QxrdDataProcessorPtr proc (expt->dataProcessor());
    QxrdCenterFinderPtr  cf   (expt->centerFinder());
    QxrdIntegratorPtr    integ(expt->integrator());

    if (proc && cf) {
      QxrdIntegratorCachePtr integCache =
          QxrdIntegratorCachePtr(new QxrdIntegratorCache(
                                   integ,
                                   qSharedPointerDynamicCast<QxrdPolarTransform>(sharedFromThis()),
                                   cf));

      if (img) {
        res = integCache->performIntegration(img, mask, 0);
      }
    }
  }

  return res;
}
