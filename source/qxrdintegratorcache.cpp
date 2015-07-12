#include "qxrddebug.h"
#include "qxrdintegratorcache.h"
#include "qxrdintegrator.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qcepintegrateddata.h"
#include "qxrddetectorgeometry.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qcepdebug.h"
#include "qcepallocator.h"
#include "qxrdintegrator.h"
#include "qxrdcenterfinder.h"

#include <stdio.h>
#include <QThread>

#define _USE_MATH_DEFINES
#include <cmath>
#include "math.h"

QxrdIntegratorCache::QxrdIntegratorCache(QxrdExperimentWPtr exp,
    QxrdIntegratorWPtr integ,
    QxrdCenterFinderWPtr cf) :
  QObject(),
  m_Oversample(1),
  m_IntegrationStep(0.001),
  m_IntegrationNSteps(0),
  m_IntegrationMinimum(0),
  m_IntegrationMaximum(100000),
  m_IntegrationXUnits(QxrdIntegrator::IntegrateTTH),
  m_CenterX(0),
  m_CenterY(0),
  m_DetectorXPixelSize(200),
  m_DetectorYPixelSize(200),
  m_DetectorDistance(1000),
  m_Energy(20000),
  m_ImplementTilt(false),
  m_DetectorTilt(0),
  m_TiltPlaneRotation(90),
  m_EnableGeometry(false),
  m_EnablePolarization(false),
  m_Polarization(1.0),
  m_EnableAbsorption(false),
  m_AttenuationLength(0.0),
  m_NRows(-1),
  m_NCols(-1),
  m_NPix(-1),
  m_RStep(0),
  m_RMin(0),
  m_RMax(0),
  m_NRange(0),
  m_Beta(0),
  m_CosBeta(1),
  m_SinBeta(0),
  m_Rot(0),
  m_CosRot(0),
  m_SinRot(0),
  m_EnableUserGeometry(0),
  m_EnableUserAbsorption(0),
  m_ScalingFactor(1.0),
  m_CacheFillLevel(-1),
  m_CacheFullLevel(-1),
  m_Experiment(exp),
  m_Integrator(integ),
  m_CenterFinder(cf)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegratorCache::QxrdIntegratorCache(%p)\n", this);
  }

  if (m_Integrator) {
    m_Oversample         = m_Integrator->get_Oversample();
    m_IntegrationStep    = m_Integrator->get_IntegrationStep();
    m_IntegrationNSteps  = m_Integrator->get_IntegrationNSteps();
    m_IntegrationMinimum = m_Integrator->get_IntegrationMinimum();
    m_IntegrationMaximum = m_Integrator->get_IntegrationMaximum();
    m_IntegrationXUnits  = m_Integrator->get_IntegrationXUnits();

    m_EnableGeometry     = m_Integrator->get_EnableGeometricCorrections();
    m_EnablePolarization = m_Integrator->get_EnablePolarizationCorrections();
    m_Polarization       = m_Integrator->get_Polarization();
    m_EnableAbsorption   = m_Integrator->get_EnableAbsorptionCorrections();
    m_AttenuationLength  = m_Integrator->get_AttenuationLength();

    m_EnableUserGeometry     = m_Integrator->get_EnableUserGeometry();
    m_UserGeometryScript     = m_Integrator->get_UserGeometryScript();
    m_UserGeometryFunction   = m_Integrator->get_UserGeometryFunction();

    m_EnableUserAbsorption   = m_Integrator->get_EnableUserAbsorption();
    m_UserAbsorptionScript   = m_Integrator->get_UserAbsorptionScript();
    m_UserAbsorptionFunction = m_Integrator->get_UserAbsorptionFunction();

    m_ScalingFactor          = m_Integrator->get_ScalingFactor();
  }

  if (m_CenterFinder) {
    m_CenterX            = m_CenterFinder->get_CenterX();
    m_CenterY            = m_CenterFinder->get_CenterY();
    m_DetectorXPixelSize = m_CenterFinder->get_DetectorXPixelSize();
    m_DetectorYPixelSize = m_CenterFinder->get_DetectorYPixelSize();
    m_DetectorDistance   = m_CenterFinder->get_DetectorDistance();
    m_Energy             = m_CenterFinder->get_Energy();
    m_ImplementTilt      = m_CenterFinder->get_ImplementTilt();
    m_DetectorTilt       = m_CenterFinder->get_DetectorTilt();
    m_TiltPlaneRotation  = m_CenterFinder->get_TiltPlaneRotation();
  }
}

QxrdIntegratorCache::~QxrdIntegratorCache()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegratorCache::~QxrdIntegratorCache(%p)\n", this);
  }
}

int QxrdIntegratorCache::get_NRows() const
{
  return m_NRows;
}

int QxrdIntegratorCache::get_NCols() const
{
  return m_NCols;
}

double QxrdIntegratorCache::getTTH(double x, double y)
{
  double beta = m_DetectorTilt*M_PI/180.0;
  double rot  = m_TiltPlaneRotation*M_PI/180.0;

  if (m_ImplementTilt) {
    return QxrdDetectorGeometry::getTwoTheta(m_CenterX, m_CenterY, m_DetectorDistance,
                                             x, y, m_DetectorXPixelSize, m_DetectorYPixelSize,
                                             cos(beta), sin(beta), cos(rot), sin(rot));
  } else {
    return QxrdDetectorGeometry::getTwoTheta(m_CenterX, m_CenterY, m_DetectorDistance,
                                             x, y, m_DetectorXPixelSize, m_DetectorYPixelSize,
                                             1.0, 0.0, 1.0, 0.0);
  }
}

double QxrdIntegratorCache::getDistance(double x, double y)
{
  double tth = getTTH(x, y)*M_PI/180.0;

  return m_DetectorDistance/cos(tth);
}

double QxrdIntegratorCache::getChi(double x, double y)
{
  double q,chi;
  double beta = m_DetectorTilt*M_PI/180.0;
  double rot  = m_TiltPlaneRotation*M_PI/180.0;

  if (m_ImplementTilt) {
    QxrdDetectorGeometry::getQChi(m_CenterX, m_CenterY, m_DetectorDistance,
                                  m_Energy,
                                  x, y, m_DetectorXPixelSize, m_DetectorYPixelSize,
                                  rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
                                  &q, &chi);
  } else {
    QxrdDetectorGeometry::getQChi(m_CenterX, m_CenterY, m_DetectorDistance,
                                  m_Energy,
                                  x, y, m_DetectorXPixelSize, m_DetectorYPixelSize,
                                  0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                  &q, &chi);
  }

  return chi;
}

double QxrdIntegratorCache::getQ(double x, double y)
{
  double q,chi;
  double beta = m_DetectorTilt*M_PI/180.0;
  double rot  = m_TiltPlaneRotation*M_PI/180.0;

  if (m_ImplementTilt) {
    QxrdDetectorGeometry::getQChi(m_CenterX, m_CenterY, m_DetectorDistance,
                                  m_Energy,
                                  x, y, m_DetectorXPixelSize, m_DetectorYPixelSize,
                                  rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
                                  &q, &chi);
  } else {
    QxrdDetectorGeometry::getQChi(m_CenterX, m_CenterY, m_DetectorDistance,
                                  m_Energy,
                                  x, y, m_DetectorXPixelSize, m_DetectorYPixelSize,
                                  0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                  &q, &chi);
  }

  return q;
}

double QxrdIntegratorCache::getR(double x, double y)
{
  double tth = getTTH(x, y);
  double r = m_DetectorDistance*tan(tth*M_PI/180.0);

  return r;
}

double QxrdIntegratorCache::XValue(double x, double y)
{
  double xVal = 0;

  if (m_EnableUserGeometry == 0) {
    switch(m_IntegrationXUnits) {
    case QxrdIntegrator::IntegrateTTH:
      xVal = getTTH(x,y);
      break;

    case QxrdIntegrator::IntegrateQ:
      xVal = getQ(x,y);
      break;

    case QxrdIntegrator::IntegrateR:
      xVal = getR(x,y);
      break;
    }
  } else {
    xVal = m_UserGeometryFunctionValue.call(QScriptValue(), QScriptValueList() << x << y).toNumber();
  }

  return xVal;
}

double QxrdIntegratorCache::NormValue(double x, double y)
{
  double res = 1;

  if (m_EnableGeometry || m_EnablePolarization || m_EnableAbsorption || m_EnableUserAbsorption) {
    double distance = getDistance(x, y);
    double tth      = getTTH(x,y)*M_PI/180.0;
    double chi      = getChi(x,y)*M_PI/180.0;

    if (m_EnableGeometry) {
      res *= pow(distance/m_DetectorDistance, 2)/cos(tth);
    }

    if (m_EnableAbsorption) {
      res *= exp((distance-m_DetectorDistance)/m_AttenuationLength);
    }

    if (m_EnablePolarization) {
      res /= (m_Polarization      *(1 - pow(sin(chi)*sin(tth/2),2)) +
              (1 - m_Polarization)*(1 - pow(cos(chi)*sin(tth/2),2)));
    }

    switch (m_EnableUserAbsorption) {
    case QxrdCenterFinder::UserAbsorptionPixelCoords:
      res *= m_UserAbsorptionFunctionValue.call(QScriptValue(), QScriptValueList() << x << y).toNumber();
      break;

    case QxrdCenterFinder::UserAbsorptionFromCenter:
      res *= m_UserAbsorptionFunctionValue.call(QScriptValue(), QScriptValueList() << x - m_CenterX << y - m_CenterY).toNumber();
      break;

    case QxrdCenterFinder::UserAbsorptionRChi:
      res *= m_UserAbsorptionFunctionValue.call(QScriptValue(), QScriptValueList() << getR(x,y) << chi).toNumber();
      break;

    case QxrdCenterFinder::UserAbsorptionQChi:
      res *= m_UserAbsorptionFunctionValue.call(QScriptValue(), QScriptValueList() << getQ(x,y) << chi).toNumber();
      break;

    default:
      break;
    }
  }

  return res;
}

class QThreadAccess : public QThread {
public:
  static void sleep(int sec)
  {
    QThread::sleep(sec);
  }

  static void usleep(int usec)
  {
    QThread::usleep(usec);
  }

  static void msleep(int msec)
  {
    QThread::msleep(msec);
  }
};

QString QxrdIntegratorCache::XLabel() const
{
  QString label = "";

  switch(m_IntegrationXUnits) {
  case QxrdIntegrator::IntegrateTTH:
    label = "2 Theta (deg)";
    break;

  case QxrdIntegrator::IntegrateQ:
    label = "Q";
    break;

  case QxrdIntegrator::IntegrateR:
    label = "r (mm)";
    break;
  }

  return label;
}

QcepIntegratedDataPtr QxrdIntegratorCache::performIntegration(
  QcepIntegratedDataPtr integ,
  QcepDoubleImageDataPtr dimg,
  QcepMaskDataPtr mask,
  int normalize)
{
  QTime tic;
  tic.start();

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    if (qcepDebug(DEBUG_INTEGRATOR)) {
      expt->printMessage(tr("QxrdIntegratorCache::performIntegration"));
    }

    if (integ && dimg) {
      int noversample = m_Oversample;
      double oversampleStep = 1.0/m_Oversample;
      double halfOversampleStep = oversampleStep/2.0;

      m_NRows = dimg->get_Height();
      m_NCols = dimg->get_Width();
      m_NPix  = m_NRows*m_NCols*m_Oversample*m_Oversample;

      m_CacheFullLevel.fetchAndStoreOrdered(m_NPix);

      QcepDoubleList norm = dimg->get_Normalization();

      double normVal = 1;

      if (norm.length()>=1) {
        normVal = norm[0];
      }

      if (m_CacheFillLevel.testAndSetOrdered(-1,0)) {
        if (qcepDebug(DEBUG_INTEGRATOR)) {
          expt->printMessage(tr("QxrdIntegratorCache::performIntegration - fill cache"));
        }

        // Allocate new cache and fill it...

//        double cx = m_CenterX;
//        double cy = m_CenterY;

        m_CachedBinNumbers =  QcepAllocator::newInt32Image(QcepAllocator::AlwaysAllocate,
                                                           m_NCols*m_Oversample,
                                                           m_NRows*m_Oversample);

        m_CachedNormalization = QcepAllocator::newDoubleImage(QcepAllocator::AlwaysAllocate,
                                                              m_NCols*m_Oversample,
                                                              m_NRows*m_Oversample);

        if (m_CachedBinNumbers && m_CachedNormalization) {
          m_CachedBinNumbers -> clear();
          m_CachedNormalization -> clear();

          if (m_EnableUserGeometry || m_EnableUserAbsorption) {
            grabScriptEngine();
          }

          double rMin=0, rMax=0;
          bool first = true;

          for (int y = 0; y < m_NRows; y++) {
            for (int x = 0; x < m_NCols; x++) {
              for (int oversampley = 0; oversampley < noversample; oversampley++) {
                double yy = y+oversampley*oversampleStep+halfOversampleStep;
                int iy = y*noversample+oversampley;
                for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
                  double xx = x+oversamplex*oversampleStep+halfOversampleStep;
                  int ix = x*noversample+oversamplex;

                  double r = XValue(xx, yy);

                  if (r == r) {
                    if (first) {
                      rMin = r;
                      rMax = r;
                      first = false;
                    } else if (r > rMax) {
                      rMax = r;
                    } else if (r < rMin) {
                      rMin = r;
                    }
                  }

                  m_CachedNormalization->setValue(ix, iy, r);
                }
              }
            }
          }

          rMin = qMax(rMin, m_IntegrationMinimum);
          rMax = qMin(rMax, m_IntegrationMaximum);

          double rStep = m_IntegrationStep;

          if (rStep == 0) {
            int nStep = m_IntegrationNSteps;

            if (nStep <= 0) {
              nStep = 512;
            }

            rStep = (rMax - rMin)/nStep;
          }

          double nMin  = floor(rMin/rStep);
          double nMax  = ceil(rMax/rStep);
          int nRange = (nMax - nMin);

          m_RStep = rStep;
          m_NRange = nRange;
          m_RMin = rMin;
          m_RMax = rMax;

          for (int y = 0; y < m_NRows; y++) {
            for (int x = 0; x < m_NCols; x++) {
              for (int oversampley = 0; oversampley < noversample; oversampley++) {
                double yy = y+oversampley*oversampleStep+halfOversampleStep;
                int iy = y*noversample+oversampley;
                for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
                  double xx = x+oversamplex*oversampleStep+halfOversampleStep;
                  int ix = x*noversample+oversamplex;

                  double r = m_CachedNormalization->value(ix, iy);
                  double n = -1;

                  if (r == r) {
                    n = floor(r / rStep);
                  }

                  if (n >= nMin && n < nMax) {
                    int bin = n - nMin;

                    m_CachedBinNumbers->setValue(ix, iy, bin);
                    m_CachedNormalization->setValue(ix, iy, NormValue(xx, yy));
                  } else {
                    m_CachedBinNumbers->setValue(ix, iy, -1);
                    m_CachedNormalization->setValue(ix, iy, 0.0);
                  }
                }
              }
            }
          }

          m_CacheFillLevel.fetchAndStoreOrdered(m_NPix);

          if (m_EnableUserGeometry || m_EnableUserAbsorption) {
            releaseScriptEngine();
          }
        }

        if (qcepDebug(DEBUG_INTEGRATOR)) {
          expt->printMessage(tr("QxrdIntegratorCache::performIntegration - cache finished"));
        }
      }

      while (m_CacheFillLevel.fetchAndAddOrdered(0) < m_CacheFullLevel.fetchAndAddOrdered(0)) {
        if (qcepDebug(DEBUG_INTEGRATOR)) {
          expt->printMessage(tr("QxrdIntegratorCache::performIntegration - waiting for cache [%1,%2]")
                             .arg(m_CacheFillLevel.fetchAndAddOrdered(0))
                             .arg(m_CacheFullLevel.fetchAndAddOrdered(0)));
        }

        QThreadAccess::msleep(100);
      }

      if (m_CacheFillLevel.fetchAndAddOrdered(0) != m_CacheFullLevel.fetchAndAddOrdered(0)) {
        expt->printMessage(tr("QxrdIntegratorCache::performIntegration - anomalous cache [%1,%2]")
                           .arg(m_CacheFillLevel.fetchAndAddOrdered(0))
                           .arg(m_CacheFullLevel.fetchAndAddOrdered(0)));
      } else {
        if (qcepDebug(DEBUG_INTEGRATOR)) {
          expt->printMessage(tr("QxrdIntegratorCache::performIntegration - use cache"));
        }

        int nRange = m_NRange;
        int nRows = m_NRows;
        int nCols = m_NCols;
        double rMin = m_RMin;
        double rStep = m_RStep;

        QVector<double> integral(nRange), sumvalue(nRange);

        int noversample = m_Oversample;

        for (int y = 0; y < nRows; y++) {
          for (int x = 0; x < nCols; x++) {
            if ((mask == NULL) || (mask->value(x, y))) {
              double val = dimg->value(x,y);
              for (int oversampley = 0; oversampley < noversample; oversampley++) {
                for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
                  int iy = y*noversample+oversampley;
                  int ix = x*noversample+oversamplex;
                  int bin = m_CachedBinNumbers->value(ix, iy);
                  double norm = m_CachedNormalization->value(ix, iy);
                  if (bin >= 0) {
                    integral[bin] += val*norm;
                    sumvalue[bin] += 1;
                  }
                }
              }
            }
          }
        }

        integ -> resize(0);
        integ -> set_Center(m_CenterX, m_CenterY);

        for(int ir=0; ir<nRange; ir++) {
          int sv = sumvalue[ir];

          if (sv > 0) {
            double xv = rMin + (ir+0.5)* /*oversampleStep+halfOversampleStep**/ rStep;

            if (normalize) {
              integ -> append(xv, m_ScalingFactor*normVal*integral[ir]/sv);
            } else {
              integ -> append(xv, m_ScalingFactor*normVal*integral[ir]/sv*(ir*oversampleStep+halfOversampleStep));
            }
          }
        }

        integ->set_XUnitsLabel(XLabel());
        integ->set_Oversample(m_Oversample);
        // Integrate entirely out of cache
      }

      expt->printMessage(tr("Integration of %1 took %2 msec")
                         .arg(dimg->get_Title())
                         .arg(tic.restart()));
    } else {
      expt->printMessage(tr("QxrdIntegratorCache::performIntegration - integration failed"));
    }
  }

  return integ;
}

void QxrdIntegratorCache::grabScriptEngine()
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    m_ScriptEngine = exp->scriptEngine();

    if (m_ScriptEngine) {
      m_ScriptEngine->lock();

      m_UserGeometryFunctionValue = QScriptValue();
      m_UserAbsorptionFunctionValue = QScriptValue();

      if (m_EnableUserGeometry) {
        m_ScriptEngine->evaluate(m_UserGeometryScript);

        m_UserGeometryFunctionValue = m_ScriptEngine->evaluate(m_UserGeometryFunction);

        if (!m_UserGeometryFunctionValue.isFunction()) {
          m_UserGeometryFunctionValue = m_ScriptEngine->globalObject().property(m_UserGeometryFunctionValue.toString());
        }

        if (m_UserGeometryFunctionValue.isFunction()) {
          exp->printMessage(tr("Using User Geometry Function %1").arg(m_UserGeometryFunctionValue.toString()));
        } else {
          exp->printMessage(tr("User Geometry Function %1 is not a function").arg(m_UserGeometryFunctionValue.toString()));
        }
      }

      if (m_EnableUserAbsorption) {
        m_ScriptEngine->evaluate(m_UserAbsorptionScript);

        m_UserAbsorptionFunctionValue = m_ScriptEngine->evaluate(m_UserAbsorptionFunction);

        if (!m_UserAbsorptionFunctionValue.isFunction()) {
          m_UserAbsorptionFunctionValue = m_ScriptEngine->globalObject().property(m_UserAbsorptionFunctionValue.toString());
        }

        if (m_UserAbsorptionFunctionValue.isFunction()) {
          exp->printMessage(tr("Using User Absorption Function %1").arg(m_UserAbsorptionFunctionValue.toString()));
        } else {
          exp->printMessage(tr("User Absorption Function %1 is not a function").arg(m_UserAbsorptionFunctionValue.toString()));
        }
      }
    }
  }
}

void QxrdIntegratorCache::releaseScriptEngine()
{
  QxrdExperimentPtr exp(m_Experiment);

  m_UserGeometryFunctionValue = QScriptValue();
  m_UserAbsorptionFunctionValue = QScriptValue();

  if (exp) {
    m_ScriptEngine = exp->scriptEngine();

    if (m_ScriptEngine) {
      m_ScriptEngine->unlock();

      if (m_EnableUserGeometry) {
        exp->printMessage(tr("User Geometry Function Completed"));
      }

      if (m_EnableUserAbsorption) {
        exp->printMessage(tr("User Absorption Function Completed"));
      }
    }
  }
}

QcepInt32ImageDataPtr QxrdIntegratorCache::cachedGeometry()
{
  return m_CachedBinNumbers;
}

QcepDoubleImageDataPtr QxrdIntegratorCache::cachedIntensity()
{
  return m_CachedNormalization;
}
