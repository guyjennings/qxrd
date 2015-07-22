#include "qxrddebug.h"
#include "qxrdintegrator.h"
#include "qxrddataprocessor.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrdcenterfinder.h"
#include "qcepintegrateddata.h"
#include "qcepallocator.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdintegratorcache.h"
#include "qcepallocator.h"
#include "qxrddataset.h"
#include "qcepdatasetmodel.h"

#include <QTime>
#include <QtConcurrentRun>
#include "qcepmutexlocker.h"

#define _USE_MATH_DEFINES

#include <cmath>

QxrdIntegrator::QxrdIntegrator(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdCenterFinderWPtr cfw)
  : QcepObject("integrator", NULL),
    m_Saver(saver),
    m_Oversample(saver, this, "oversample", 1, "Oversampling for Integration"),
    m_IntegrationStep(saver, this, "integrationStep", 0.001, "Integration Step Size"),
    m_IntegrationNSteps(saver, this, "integrationNSteps", 0, "Integration Number of Steps"),
    m_IntegrationMinimum(saver, this, "integrationMinimum", 0, "Integration Minimum"),
    m_IntegrationMaximum(saver, this, "integrationMaximum", 100000, "Integration Maximum"),
    m_IntegrationXUnits(saver, this, "integrationXUnits", IntegrateTTH, "X Units for Integration (0 = TTH, 1 = Q, 2 = R)"),
    m_EnableGeometricCorrections(saver, this, "enableGeometricCorrections", false, "Enable Geometric Corrections (tilt and distance) in Integration"),
    m_EnablePolarizationCorrections(saver, this, "enablePolarizationCorrections", false, "Enable Polarization Corrections in Integration"),
    m_Polarization(saver, this, "polarization", 1.0, "Beam Polarization Factor"),
    m_EnableAbsorptionCorrections(saver, this, "enableAbsorptionCorrections", false, "Enable Absorption Correction in Integration"),
    m_AttenuationLength(saver, this, "attenuationLength", 0, "Attenuation Length (mm)"),
    m_EnableUserGeometry(saver, this, "enableUserGeometry", 0, "Apply user-defined geometry function in integration"),
    m_UserGeometryScript(saver, this, "userGeometryScript", defaultUserGeometryScript(), "Script to define user defined geometry functions"),
    m_UserGeometryFunction(saver, this, "userGeometryFunction", "userGeometry", "Name of user defined geometry function"),
    m_EnableUserAbsorption(saver, this, "enableUserAbsorption", 0, "Apply user-defined geometry function in integration"),
    m_UserAbsorptionScript(saver, this, "userAbsorptionScript", defaultUserAbsorptionScript(), "Script to define user defined absorption functions"),
    m_UserAbsorptionFunction(saver, this, "userAbsorptionFunction", "userAbsorb1", "Name of user defined absorption function"),
    m_ScalingFactor(saver, this, "scalingFactor", 1.0, "Scaling factor for integrated intensity"),
    m_SelfNormalization(saver, this, "selfNormalization", false, "Normalize result based on average value within specified range"),
    m_SelfNormalizationMinimum(saver, this, "selfNormalizationMinimum", 0, "Self Normalization Range Minimum"),
    m_SelfNormalizationMaximum(saver, this, "selfNormalizationMaximum", 0, "Self Normalization Range Maximum"),
    m_Experiment(exp),
    m_CenterFinder(cfw),
    m_IntegratorCache()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegrator::QxrdIntegrator(%p)\n", this);
  }

  connect(this->prop_Oversample(),         SIGNAL(valueChanged(int,int)),    this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(this->prop_IntegrationStep(),    SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(this->prop_IntegrationNSteps(),  SIGNAL(valueChanged(int,int)),    this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(this->prop_IntegrationMinimum(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(this->prop_IntegrationMaximum(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(this->prop_IntegrationXUnits(),  SIGNAL(valueChanged(int,int)),    this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);

  connect(prop_EnableGeometricCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_EnablePolarizationCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_Polarization(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_EnableAbsorptionCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_AttenuationLength(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);

  connect(prop_EnableUserGeometry(), SIGNAL(valueChanged(int,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_UserGeometryScript(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_UserGeometryFunction(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);

  connect(prop_EnableUserAbsorption(), SIGNAL(valueChanged(int,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_UserAbsorptionScript(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_UserAbsorptionFunction(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);

  connect(prop_ScalingFactor(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_SelfNormalization(), SIGNAL(valueChanged(bool,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_SelfNormalizationMinimum(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  connect(prop_SelfNormalizationMaximum(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);

  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    connect(cf.data(), SIGNAL(parameterChanged()), this, SLOT(onIntegrationParametersChanged()), Qt::DirectConnection);
  }
}

QxrdIntegrator::~QxrdIntegrator()
{
#ifndef QT_NO_DEBUG
  printf("Deleting integrator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegrator::~QxrdIntegrator(%p)\n", this);
  }
}

QxrdDataProcessorWPtr QxrdIntegrator::dataProcessor() const
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    return expt->dataProcessor();
  } else {
    return QxrdDataProcessorWPtr();
  }
}

QxrdExperimentWPtr QxrdIntegrator::experiment() const
{
  return m_Experiment;
}

void QxrdIntegrator::onIntegrationParametersChanged()
{
  if (qcepDebug(DEBUG_INTEGRATOR)) {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      expt->printMessage("Integration parameters changed");
    }
  }

  m_IntegratorCache = QxrdIntegratorCachePtr();
}

QcepIntegratedDataPtr QxrdIntegrator::performIntegration(QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask)
{
  QcepIntegratedDataPtr res = QcepAllocator::newIntegratedData(QcepAllocator::AlwaysAllocate, dimg);

  if (res) {
    return performIntegration(res, dimg, mask);
  } else {
    return res;
  }
}

QcepIntegratedDataPtr QxrdIntegrator::performIntegration(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask)
{
  if (qcepDebug(DEBUG_INTEGRATOR)) {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      expt->printMessage(tr("QxrdIntegrator::performIntegration(\"%1\")")
                         .arg(dimg->get_FileName()));
    }
  }

  QThread::currentThread()->setObjectName("performIntegration");

  QxrdIntegratorCachePtr cache = m_IntegratorCache;

  if (cache == NULL ||
      dimg->get_Width() != cache->get_NCols() ||
      dimg->get_Height() != cache->get_NRows()) {

    cache = QxrdIntegratorCachePtr(
          new QxrdIntegratorCache(m_Experiment, sharedFromThis(), m_CenterFinder));

    m_IntegratorCache = cache;
  }

  return cache->performIntegration(integ, dimg, mask, true);
}

double QxrdIntegrator::XValue(QPointF pt) const
{
  return XValue(pt.x(), pt.y());
}

double QxrdIntegrator::XValue(double x, double y) const
{
  double xVal = 0;

  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    switch(get_IntegrationXUnits()) {
    case IntegrateTTH:
      xVal = cf->getTTH(x,y);
      break;

    case IntegrateQ:
      xVal = cf->getQ(x,y);
      break;

    case IntegrateR:
      xVal = cf->getR(x,y);
      break;
    }
  }

  return xVal;
}

double QxrdIntegrator::XValue(double x, double y,
                              int xUnits, QxrdCenterFinderPtr cf,
                              double xc, double yc,
                              double dst, double nrg,
                              double pxl, double pxh,
                              double rot, double cosr, double sinr,
                              double cosb, double sinb,
                              double cosa, double sina
                              ) const
{
  double xVal = 0;
  double junk;

  switch(xUnits) {
  case IntegrateTTH:
    xVal = cf->getTwoTheta(xc,yc,dst,x,y,pxl,pxh,cosb,sinb,cosr,sinr);
    break;

  case IntegrateQ:
    cf->getQChi(xc,yc,dst,nrg,x,y,pxl,pxh,rot,cosb,sinb,cosa,sina,cosr,sinr,&xVal, &junk);
    break;

  case IntegrateR:
    xVal = cf->getRadius(xc,yc,dst,x,y,pxl,pxh,cosb,sinb,cosr,sinr);
    break;
  }

  return xVal;
}

QString QxrdIntegrator::XLabel() const
{
  QString label = "";

  switch(get_IntegrationXUnits()) {
  case IntegrateTTH:
    label = "2 Theta (deg)";
    break;

  case IntegrateQ:
    label = "Q";
    break;

  case IntegrateR:
    label = "r (mm)";
    break;
  }

  return label;
}

QcepIntegratedDataPtr QxrdIntegrator::sliceLine(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr image, double x0, double y0, double x1, double y1, double width)
{
  try {
    QVector<QPointF> poly;
    poly.append(QPointF(x0,y0));
    poly.append(QPointF(x1,y1));

    return slicePolygon(integ, image, poly, width);
  }

  catch (...) {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      expt->printMessage("QxrdIntegrator::sliceLine failed");
    }
  }

  return QcepIntegratedDataPtr();
}

QcepIntegratedDataPtr QxrdIntegrator::slicePolygon(QcepIntegratedDataPtr integ, QcepDoubleImageDataPtr image, QVector<QPointF> poly, double /*width*/)
{
  QThread::currentThread()->setObjectName("slicePolygon");

  if (integ && image) {
    double length = 0;

    if (poly.size() >= 2) {
      QPointF p0 = poly[0];

      for (int i=1; i<poly.size(); i++) {
        QPointF p1 = poly[i];
        double dx = p1.x() - p0.x();
        double dy = p1.y() - p0.y();
        length += sqrt(dx*dx + dy*dy);
        p0=p1;
      }

      p0 = poly[0];
      double r = 0;
      double r0 = 0;

      //    QVector<double> xs,ys;
      integ -> resize(0);

      for (int i=1; i<poly.size(); i++) {
        QPointF p1 = poly[i];
        double dx = p1.x() - p0.x();
        double dy = p1.y() - p0.y();
        double len = sqrt(dx*dx + dy*dy);

        if (len > 0) {
          for (; r<len; r+=1) {
            double x = p0.x() + r*dx/len;
            double y = p0.y() + r*dy/len;

            integ -> append(r+r0, image->value((int) x, (int) y));
          }
        }

        p0 = p1;
        r0 += len;
        r  -= len;
      }
      //
      //    emit newIntegrationAvailable(image->get_Title(),xs,ys);
    }
  } else {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      expt->printMessage("QxrdIntegrator::slicePolygon failed");
    }
  }

  return integ;
}

void QxrdIntegrator::integrateVsQ()
{
  set_IntegrationXUnits(IntegrateQ);
}

void QxrdIntegrator::integrateVsR()
{
  set_IntegrationXUnits(IntegrateR);
}

void QxrdIntegrator::integrateVsTTH()
{
  set_IntegrationXUnits(IntegrateTTH);
}

QcepInt32ImageDataPtr QxrdIntegrator::cachedGeometry()
{
  if (m_IntegratorCache) {
    return m_IntegratorCache->cachedGeometry();
  } else {
    return QcepInt32ImageDataPtr();
  }
}

QcepDoubleImageDataPtr QxrdIntegrator::cachedIntensity()
{
  if (m_IntegratorCache) {
    return m_IntegratorCache->cachedIntensity();
  } else {
    return QcepDoubleImageDataPtr();
  }
}

QString QxrdIntegrator::defaultUserGeometryScript()
{
  QFile def(":/qxrdexampleusergeometry.js");

  if (def.open(QFile::ReadOnly)) {
    return def.readAll();
  } else {
    return "Couldn't open resource file";
  }
}

QString QxrdIntegrator::defaultUserAbsorptionScript()
{
  QFile def(":/qxrdexampleuserabsorption.js");

  if (def.open(QFile::ReadOnly)) {
    return def.readAll();
  } else {
    return "Couldn't open resource file";
  }
}

void QxrdIntegrator::appendIntegration(QString resPath, QcepIntegratedDataPtr integ)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepDatasetModelPtr ds = expt->dataset();

    if (ds) {
      QcepDoubleImageDataPtr data = ds->image(resPath);

      if (!data) {
        data = ds->newImage(resPath);
      }

      if (data) {
        appendIntegration(data, integ);
      } else {
        printMessage(tr("Unable to accumulate integrated data into %1").arg(resPath));
      }
    }
  }
}

void QxrdIntegrator::appendIntegration(QString resPath, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepDatasetModelPtr ds = expt->dataset();

    if (ds) {
      QcepDoubleImageDataPtr data = ds->image(resPath);

      if (!data) {
        data = ds->newImage(resPath);
      }

      if (data) {
        appendIntegration(data, dimg, mask);
      }
    }
  }
}

void QxrdIntegrator::appendIntegration(QcepDoubleImageDataPtr res, QcepDoubleImageDataPtr dimg, QcepMaskDataPtr mask)
{
  QcepIntegratedDataPtr integ = performIntegration(dimg, mask);

  appendIntegration(res, integ);
}

void QxrdIntegrator::appendIntegration(QcepDoubleImageDataPtr res, QcepIntegratedDataPtr integ)
{
  if (res && integ) {
    int width = res->get_Width();
    int ht    = res->get_Height();
    int npts  = integ->size();

    res->resize(npts, ht+1);

    for (int i=0; i<npts; i++) {
      res->setValue(i, ht, integ->y(i));
    }

    emit res->dataObjectChanged();
  }
}

void QxrdIntegrator::clearAccumulator(QString resPath)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepDatasetModelPtr ds = expt->dataset();

    if (ds) {
      QcepDoubleImageDataPtr data = ds->image(resPath);

      if (!data) {
        data = ds->newImage(resPath);
      }

      if (data) {
        data->resize(0, 0);
      }
    }
  }
}

void QxrdIntegrator::prepareAccumulator(QString resPath, int nImages)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepDatasetModelPtr ds = expt->dataset();

    if (ds) {
      QcepDoubleImageDataPtr data = ds->image(resPath);

      if (!data) {
        data = ds->newImage(resPath);
      }
    }
  }
}

void QxrdIntegrator::completeAccumulator(QString path)
{
}

void QxrdIntegrator::saveAccumulator(QString resPath, QString &fileName, QString filter)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepDatasetModelPtr ds = expt->dataset();

    if (ds) {
      QcepDoubleImageDataPtr data = ds->image(resPath);

      if (data) {
        data -> saveData(fileName, filter);
      }
    }
  }
}
