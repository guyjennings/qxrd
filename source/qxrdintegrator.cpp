#include "qxrddebug.h"
#include "qxrdintegrator.h"
#include "qxrddataprocessor.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrateddata.h"
#include "qxrdallocator.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qxrdintegratorcache.h"

#include <QTime>
#include <QtConcurrentRun>
#include "qxrdmutexlocker.h"

#define _USE_MATH_DEFINES

#include <cmath>

QxrdIntegrator::QxrdIntegrator(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdCenterFinderWPtr cfw, QxrdAllocatorWPtr alloc)
  : QObject(NULL),
    m_ObjectNamer(this, "integrator"),
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
    m_Experiment(exp),
    m_CenterFinder(cfw),
    m_Allocator(alloc),
    m_IntegratorCache()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegrator::QxrdIntegrator(%p)\n", this);
  }

  connect(this->prop_Oversample(),         SIGNAL(valueChanged(int,int)),    this, SLOT(onIntegrationParametersChanged()));
  connect(this->prop_IntegrationStep(),    SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(this->prop_IntegrationNSteps(),  SIGNAL(valueChanged(int,int)),    this, SLOT(onIntegrationParametersChanged()));
  connect(this->prop_IntegrationMinimum(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(this->prop_IntegrationMaximum(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(this->prop_IntegrationXUnits(),  SIGNAL(valueChanged(int,int)),    this, SLOT(onIntegrationParametersChanged()));

  connect(prop_EnableGeometricCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_EnablePolarizationCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_Polarization(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_EnableAbsorptionCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_AttenuationLength(), SIGNAL(valueChanged(double,int)), this, SLOT(onIntegrationParametersChanged()));

  connect(prop_EnableUserGeometry(), SIGNAL(valueChanged(int,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_UserGeometryScript(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_UserGeometryFunction(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()));

  connect(prop_EnableUserAbsorption(), SIGNAL(valueChanged(int,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_UserAbsorptionScript(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()));
  connect(prop_UserAbsorptionFunction(), SIGNAL(valueChanged(QString,int)), this, SLOT(onIntegrationParametersChanged()));

  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    connect(cf.data(), SIGNAL(parameterChanged()), this, SLOT(onIntegrationParametersChanged()));
  }
}

void QxrdIntegrator::initialize(QxrdIntegratorWPtr integrator)
{
  m_Integrator = integrator;
}

QxrdIntegrator::~QxrdIntegrator()
{
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

void QxrdIntegrator::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdIntegrator::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
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

QxrdIntegratedDataPtr QxrdIntegrator::performIntegration(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask)
{
  QThread::currentThread()->setObjectName("performIntegration");

  QxrdIntegratorCachePtr cache = m_IntegratorCache;

  if (cache == NULL ||
      dimg->get_Width() != cache->get_NCols() ||
      dimg->get_Height() != cache->get_NRows()) {

    cache = QxrdIntegratorCachePtr(
          new QxrdIntegratorCache(m_Experiment, m_Allocator, m_Integrator, m_CenterFinder));

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

QxrdIntegratedDataPtr QxrdIntegrator::sliceLine(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr image, double x0, double y0, double x1, double y1, double width)
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

  return QxrdIntegratedDataPtr();
}

QxrdIntegratedDataPtr QxrdIntegrator::slicePolygon(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr image, QVector<QPointF> poly, double /*width*/)
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

QxrdInt32ImageDataPtr QxrdIntegrator::cachedGeometry()
{
  if (m_IntegratorCache) {
    return m_IntegratorCache->cachedGeometry();
  } else {
    return QxrdInt32ImageDataPtr();
  }
}

QxrdDoubleImageDataPtr QxrdIntegrator::cachedIntensity()
{
  if (m_IntegratorCache) {
    return m_IntegratorCache->cachedIntensity();
  } else {
    return QxrdDoubleImageDataPtr();
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

