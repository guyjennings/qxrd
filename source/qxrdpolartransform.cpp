#include "qxrdpolartransform.h"
#include "qxrdexperiment.h"
#include "qxrdintegratorcache.h"
#include "qxrddataprocessor.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdatasetmodel.h"
#include "qcepimagedata.h"

QxrdPolarTransform::QxrdPolarTransform(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp) :
  QcepObject("polarTransform", NULL),
  m_Destination(saver, this, "destination", "Polar/image", "Destination for polar transform"),
  m_OutputType(saver, this, "outputType", 3, "Output type 0=none, 1=data, 2=radial plot, 3=polar plot"),
  m_Oversample(saver, this, "oversample", 1, "Oversample factor"),
  m_PolarUnits(saver, this, "polarUnits", QxrdIntegrator::PolarIntegrateChi, "Polar Integration Type (0=Angle)"),
  m_PolarStep(saver, this, "polarStep", 0.5, "Polar Integration Step Size"),
  m_PolarNSteps(saver, this, "polarNSteps", 0, "Polar Integration Number of Steps"),
  m_PolarStart(saver, this, "polarStart", 0, "Polar Integration Start"),
  m_PolarEnd(saver, this, "polarEnd", 360, "Polar Integration End"),
  m_RadialUnits(saver, this, "radialUnits", QxrdIntegrator::RadialIntegrateTTH, "Radial Units for Integration (0 = TTH, 1 = Q, 2 = R)"),
  m_RadialStep(saver, this, "radialStep", 0.001, "Radial Integration Step Size"),
  m_RadialNSteps(saver, this, "radialNSteps", 0, "Radial Integration Number of Steps"),
  m_RadialStart(saver, this, "radialStart", 0, "Radial Integration Start"),
  m_RadialEnd(saver, this, "radialEnd", 100000, "Radial Integration End"),
  m_Saver(saver),
  m_Experiment(exp)
{
}

QxrdPolarTransform::~QxrdPolarTransform()
{
}

QxrdIntegratorWPtr QxrdPolarTransform::integrator() const
{
  QxrdIntegratorWPtr res;

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    res = expt->integrator();
  }

  return res;
}

void QxrdPolarTransform::execute()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdDataProcessorPtr proc(expt->dataProcessor());
    QxrdCenterFinderPtr  cf(expt->centerFinder());
    QxrdIntegratorPtr    integ(expt->integrator());

    if (proc && cf) {
      QcepDoubleImageDataPtr img  = proc->data();
      QcepMaskDataPtr        mask = proc->mask();

      m_IntegratorCache =
          QxrdIntegratorCachePtr(new QxrdIntegratorCache(
                                   expt, integ, sharedFromThis(), cf));

      QcepDatasetModelPtr    ds  = expt->dataset();

      QcepDoubleImageDataPtr res = ds->newImage(get_Destination());

      m_IntegratorCache->performIntegration(res, img, mask, 0);
    }
  }
}
