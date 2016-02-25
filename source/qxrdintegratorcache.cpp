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
#include "qxrdpolartransform.h"
#include "qxrdcenterfinder.h"
#include "qcepmutexlocker.h"
#include "qcepdataobject.h"

#include <stdio.h>
#include <QThread>

#define _USE_MATH_DEFINES
#include <cmath>
#include "math.h"

#include <QtConcurrentRun>

QxrdIntegratorCache::QxrdIntegratorCache
  (QxrdExperimentWPtr exp,
   QxrdIntegratorWPtr integ,
   QxrdPolarTransformWPtr xform,
   QxrdCenterFinderWPtr cf) :
  QObject(),
  m_ThreadCount(QThreadPool::globalInstance()->maxThreadCount()),
  m_Oversample(1),
  m_RadialStep(0.001),
  m_RadialNSteps(0),
  m_RadialStart(0),
  m_RadialEnd(100000),
  m_RadialUnits(QxrdIntegrator::RadialIntegrateTTH),
  m_PolarStep(0.5),
  m_PolarNSteps(0),
  m_PolarStart(0),
  m_PolarEnd(360),
  m_PolarUnits(QxrdIntegrator::PolarIntegrateNone),
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
  m_HasChi(false),
  m_Experiment(exp),
  m_Integrator(integ),
  m_PolarTransform(xform),
  m_CenterFinder(cf)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdIntegratorCache::QxrdIntegratorCache(%p)\n", this);
  }

  QxrdIntegratorPtr     integp(m_Integrator);
  QxrdPolarTransformPtr xformp(m_PolarTransform);

  if (integp == NULL && xformp) {
    integp = xformp->integrator();
  }

  if (integp) {
    m_Oversample         = integp->get_Oversample();
    m_RadialStep    = integp->get_IntegrationStep();
    m_RadialNSteps  = integp->get_IntegrationNSteps();
    m_RadialStart = integp->get_IntegrationMinimum();
    m_RadialEnd = integp->get_IntegrationMaximum();
    m_RadialUnits  = integp->get_IntegrationXUnits();

    m_EnableGeometry     = integp->get_EnableGeometricCorrections();
    m_EnablePolarization = integp->get_EnablePolarizationCorrections();
    m_Polarization       = integp->get_Polarization();
    m_EnableAbsorption   = integp->get_EnableAbsorptionCorrections();
    m_AttenuationLength  = integp->get_AttenuationLength();

    m_EnableUserGeometry     = integp->get_EnableUserGeometry();
    m_UserGeometryScript     = integp->get_UserGeometryScript();
    m_UserGeometryFunction   = integp->get_UserGeometryFunction();

    m_EnableUserAbsorption   = integp->get_EnableUserAbsorption();
    m_UserAbsorptionScript   = integp->get_UserAbsorptionScript();
    m_UserAbsorptionFunction = integp->get_UserAbsorptionFunction();

    m_ScalingFactor          = integp->get_ScalingFactor();

    m_SelfNormalization      = integp->get_SelfNormalization();
    m_SelfNormalizationMinimum = integp->get_SelfNormalizationMinimum();
    m_SelfNormalizationMaximum = integp->get_SelfNormalizationMaximum();
  }

  if (xformp) {
    m_RadialStep   = xformp->get_RadialStep();
    m_RadialNSteps = xformp->get_RadialNSteps();
    m_RadialStart  = xformp->get_RadialStart();
    m_RadialEnd    = xformp->get_RadialEnd();
    m_RadialUnits  = xformp->get_RadialUnits();

    m_PolarStep    = xformp->get_PolarStep();
    m_PolarNSteps  = xformp->get_PolarNSteps();
    m_PolarStart   = xformp->get_PolarStart();
    m_PolarEnd     = xformp->get_PolarEnd();
    m_PolarUnits   = xformp->get_PolarUnits();

    m_HasChi       = true;
  }

  QxrdCenterFinderPtr cfp(m_CenterFinder);

  if (cfp) {
    m_CenterX            = cfp->get_CenterX();
    m_CenterY            = cfp->get_CenterY();
    m_DetectorXPixelSize = cfp->get_DetectorXPixelSize();
    m_DetectorYPixelSize = cfp->get_DetectorYPixelSize();
    m_DetectorDistance   = cfp->get_DetectorDistance();
    m_Energy             = cfp->get_Energy();
    m_ImplementTilt      = cfp->get_ImplementTilt();
    m_DetectorTilt       = cfp->get_DetectorTilt();
    m_TiltPlaneRotation  = cfp->get_TiltPlaneRotation();
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
    switch(m_RadialUnits) {
    case QxrdIntegrator::RadialIntegrateTTH:
      xVal = getTTH(x,y);
      break;

    case QxrdIntegrator::RadialIntegrateQ:
      xVal = getQ(x,y);
      break;

    case QxrdIntegrator::RadialIntegrateR:
      xVal = getR(x,y);
      break;
    }
  } else {
    xVal = m_UserGeometryFunctionValue.call(QScriptValue(), QScriptValueList() << x << y).toNumber();
  }

  return xVal;
}

double QxrdIntegratorCache::YValue(double x, double y)
{
  double yVal = 0;

  switch (m_PolarUnits) {
  case QxrdIntegrator::PolarIntegrateChi:
    yVal = getChi(x,y);
  }

  return yVal;
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

  switch(m_RadialUnits) {
  case QxrdIntegrator::RadialIntegrateTTH:
    label = "2 Theta (deg)";
    break;

  case QxrdIntegrator::RadialIntegrateQ:
    label = "Q";
    break;

  case QxrdIntegrator::RadialIntegrateR:
    label = "r (mm)";
    break;
  }

  return label;
}

int QxrdIntegratorCache::resultSize() const
{
  int nRange = 0;

  int nStride = m_NRMax - m_NRMin;

  if (m_CachedPolarBinNumbers) {
    int ncRange = m_NCMax - m_NCMin;
    nRange = nStride*ncRange;
  } else {
    nRange = nStride;
  }

  return nRange;
}

void QxrdIntegratorCache::partialIntegrationStep1(int i, int n)
{
  int strideSize = m_NRows / m_ThreadCount;

  while (strideSize*m_ThreadCount < m_NRows) {
    strideSize++;
  }

  int rowStart = strideSize*i;
  int rowEnd   = rowStart + strideSize;

  if (rowEnd > m_NRows) {
    rowEnd = m_NRows;
  }

  int noversample = m_Oversample;
  double oversampleStep = 1.0/m_Oversample;
  double halfOversampleStep = oversampleStep/2.0;

  double rMin=0, rMax=0, cMin=0, cMax=0;
  bool rFirst=true, cFirst=true;

  for(int y = rowStart; y<rowEnd; y++) {
    for (int x = 0; x < m_NCols; x++) {
      for (int oversampley = 0; oversampley < noversample; oversampley++) {
        double yy = y+oversampley*oversampleStep+halfOversampleStep;
        int iy = y*noversample+oversampley;
        for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
          double xx = x+oversamplex*oversampleStep+halfOversampleStep;
          int ix = x*noversample+oversamplex;

          double r = XValue(xx, yy);

          if (r==r) {
            if (rFirst) {
              rMin = r;
              rMax = r;
              rFirst = false;
            } else if (r > rMax) {
              rMax = r;
            } else if (r < rMin) {
              rMin = r;
            }
          }

          m_CachedRadialValues->setValue(ix, iy, r);

          if (m_HasChi) {
            double c = YValue(xx, yy);

            if (c==c) {
              if (cFirst) {
                cMin = c;
                cMax = c;
                cFirst = false;
              } else if (c > cMax) {
                cMax = c;
              } else if (c < cMin) {
                cMin = c;
              }
            }

            m_CachedPolarValues->setValue(ix, iy, c);
          }
        }
      }
    }
  }

  if (rFirst == false || cFirst == false) {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    if (rFirst == false) {
      if (m_RFirst) {
        m_RMin = rMin;
        m_RMax = rMax;
        m_RFirst = false;
      } else {
        m_RMin = qMin(m_RMin, rMin);
        m_RMax = qMax(m_RMax, rMax);
      }

      m_RStep = m_RadialStep;

      if (m_RStep == 0) {
        int nStep = m_RadialNSteps;

        if (nStep <= 0) {
          nStep = 512;
        }

        m_RStep = (m_RMax - m_RMin)/nStep;
      }

      m_NRMin = floor(m_RMin / m_RStep);
      m_NRMax = ceil(m_RMax / m_RStep);
    }

    if (cFirst == false) {
      if (m_CFirst) {
        m_CMin = cMin;
        m_CMax = cMax;
        m_CFirst = false;
      } else {
        m_CMin = qMin(m_CMin, cMin);
        m_CMax = qMax(m_CMax, cMax);
      }

      m_CStep = m_PolarStep;

      if (m_CStep == 0) {
        int nStep = m_PolarNSteps;

        if (nStep <= 0) {
          nStep = 360;
        }

        m_CStep = (m_CMax - m_CMin)/nStep;
      }

      m_NCMin = floor(m_CMin / m_CStep);
      m_NCMax = ceil(m_CMax / m_CStep);
    }
  }
}

void QxrdIntegratorCache::partialIntegrationStep2(int i, int n)
{
  int strideSize = m_NRows / m_ThreadCount;

  while (strideSize*m_ThreadCount < m_NRows) {
    strideSize++;
  }

  int rowStart = strideSize*i;
  int rowEnd   = rowStart + strideSize;

  if (rowEnd > m_NRows) {
    rowEnd = m_NRows;
  }

  int noversample = m_Oversample;
  double oversampleStep = 1.0/m_Oversample;
  double halfOversampleStep = oversampleStep/2.0;

  double rMin = (m_RFirst ? 0 : qMax(m_RMin, m_RadialStart));
  double rMax = (m_RFirst ? 1 : qMin(m_RMax, m_RadialEnd));

  double rStep = m_RadialStep;

  if (rStep == 0) {
    int nStep = m_RadialNSteps;

    if (nStep <= 0) {
      nStep = 512;
    }

    rStep = (rMax - rMin)/nStep;
  }

  double cMin, cMax, cStep;

  if (m_HasChi) {
    cMin = (m_CFirst ? 0   : qMax(m_CMin, m_PolarStart));
    cMax = (m_CFirst ? 360 : qMin(m_CMax, m_PolarEnd));

    cStep = m_PolarStep;

    if (cStep == 0) {
      int nStep = m_PolarNSteps;

      if (nStep <= 0) {
        nStep = 512;
      }

      cStep = (cMax - cMin)/nStep;
    }
  }

  for(int y = rowStart; y<rowEnd; y++) {
    for (int x = 0; x < m_NCols; x++) {
      for (int oversampley = 0; oversampley < noversample; oversampley++) {
        double yy = y+oversampley*oversampleStep+halfOversampleStep;
        int iy = y*noversample+oversampley;
        for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
          double xx = x+oversamplex*oversampleStep+halfOversampleStep;
          int ix = x*noversample+oversamplex;

          double r = m_CachedRadialValues->value(ix, iy);
          double n = -1;

          if (r == r) {
            n = floor(r / m_RStep);
          }

          if (n >= m_NRMin && n < m_NRMax) {
            int bin = n - m_NRMin;

            m_CachedRadialBinNumbers->setValue(ix, iy, bin);
            m_CachedNormalization->setValue(ix, iy, NormValue(xx, yy));
          } else {
            m_CachedRadialBinNumbers->setValue(ix, iy, -1);
            m_CachedNormalization->setValue(ix, iy, 0.0);
          }

          if (m_HasChi) {
            double chi = m_CachedPolarValues->value(ix, iy);
            double n   = -1;

            if (chi == chi) {
              n = floor(chi/m_CStep);
            }

            if (n >= m_NCMin && n < m_NCMax) {
              int bin = n - m_NCMin;

              m_CachedPolarBinNumbers->setValue(ix, iy, bin);
            } else {
              m_CachedPolarBinNumbers->setValue(ix, iy, -1);
            }
          }
        }
      }
    }
  }
}

void QxrdIntegratorCache::partialIntegrationStep3(
    int i,
    int n,
    QcepDoubleImageDataPtr dimg,
    QcepMaskDataPtr mask,
    int normalize)
{
  int strideSize = m_NRows / m_ThreadCount;

  while (strideSize*m_ThreadCount < m_NRows) {
    strideSize++;
  }

  int rowStart = strideSize*i;
  int rowEnd   = rowStart + strideSize;

  if (rowEnd > m_NRows) {
    rowEnd = m_NRows;
  }

  int noversample = m_Oversample;
  double oversampleStep = 1.0/m_Oversample;
  double halfOversampleStep = oversampleStep/2.0;

  int nRange = 0;

  int nStride = m_NRMax - m_NRMin;

  if (m_HasChi) {
    int ncRange = m_NCMax - m_NCMin;
    nRange = nStride*ncRange;
  } else {
    nRange = nStride;
  }

  QVector<double> integral(nRange), sumValue(nRange);

  for(int y = rowStart; y<rowEnd; y++) {
    for (int x = 0; x < m_NCols; x++) {
      if ((mask == NULL) || (mask->value(x, y))) {
        double val = dimg->value(x,y);
        for (int oversampley = 0; oversampley < noversample; oversampley++) {
          double yy = y+oversampley*oversampleStep+halfOversampleStep;
          int iy = y*noversample+oversampley;
          for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
            double xx = x+oversamplex*oversampleStep+halfOversampleStep;
            int    ix = x*noversample+oversamplex;

            int    rbin = m_CachedRadialBinNumbers->value(ix,iy);
            double norm = m_CachedNormalization->value(ix,iy);

            int    bin  = -1;

            if (m_HasChi) {
              int cbin = m_CachedPolarBinNumbers->value(ix,iy);

              if (cbin >= 0) {
                bin = cbin*nStride + rbin;
              }
            } else {
              bin = rbin;
            }

            if (bin >= 0) {
              integral[bin] += val*norm;
              sumValue[bin] += 1;
            }
          }
        }
      }
    }
  }

  {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    double *s1 = m_SumValue.data();
    double *s2 = sumValue.data();
    double *v1 = m_Integral.data();
    double *v2 = integral.data();

    for (int i=0; i<nRange; i++) {
      if (s2[i] > 0) {
        v1[i] += v2[i];
        s1[i] += s2[i];
      }
    }
  }
}

void QxrdIntegratorCache::performIntegration(
  QcepDataObjectPtr      res,
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

    m_Integral.resize(0);
    m_SumValue.resize(0);

    if (res && dimg) {
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

        m_CachedRadialBinNumbers =  QcepAllocator::newInt32Image(QcepAllocator::AlwaysAllocate,
                                                           m_NCols*m_Oversample,
                                                           m_NRows*m_Oversample, expt.data());

        m_CachedRadialValues = QcepAllocator::newDoubleImage(QcepAllocator::AlwaysAllocate,
                                                              m_NCols*m_Oversample,
                                                              m_NRows*m_Oversample, expt.data());

        m_CachedNormalization = QcepAllocator::newDoubleImage(QcepAllocator::AlwaysAllocate,
                                                              m_NCols*m_Oversample,
                                                              m_NRows*m_Oversample, expt.data());

        if (m_PolarTransform) {
          m_CachedPolarBinNumbers = QcepAllocator::newInt32Image(QcepAllocator::AlwaysAllocate,
                                                                 m_NCols*m_Oversample,
                                                                 m_NRows*m_Oversample, expt.data());

          m_CachedPolarValues = QcepAllocator::newDoubleImage(QcepAllocator::AlwaysAllocate,
                                                              m_NCols*m_Oversample,
                                                              m_NRows*m_Oversample, expt.data());
        }

        if (m_CachedRadialBinNumbers && m_CachedNormalization) {
          m_CachedRadialBinNumbers -> clear();
          m_CachedNormalization -> clear();

          if (m_CachedPolarBinNumbers) {
            m_CachedPolarBinNumbers -> clear();
          }

          if (m_CachedPolarValues) {
            m_CachedPolarValues -> clear();
          }

          if (m_EnableUserGeometry || m_EnableUserAbsorption) {
            expt->printMessage(tr("Threaded integration disabled because of user supplied functions"));

            m_ThreadCount = 1;

            grabScriptEngine();
          }

          m_RFirst = true;
          m_CFirst = true;

          if (m_ThreadCount == 1) {
            partialIntegrationStep1(0, 1);
          } else {
            QVector< QFuture<void> > res;

            for (int i=0; i<m_ThreadCount; i++) {
              res.append(QtConcurrent::run(this, &QxrdIntegratorCache::partialIntegrationStep1, i, m_ThreadCount));
            }

            for (int i=0; i<m_ThreadCount; i++) {
              res[i].waitForFinished();
            }
          }

          if (qcepDebug(DEBUG_INTEGRATOR)) {
            expt->printMessage(tr("1st stage complete after %1 msec").arg(tic.elapsed()));
          }

          if (m_ThreadCount == 1) {
            partialIntegrationStep2(0, 1);
          } else {
            QVector< QFuture<void> > res;

            for (int i=0; i<m_ThreadCount; i++) {
              res.append(QtConcurrent::run(this, &QxrdIntegratorCache::partialIntegrationStep2, i, m_ThreadCount));
            }

            for (int i=0; i<m_ThreadCount; i++) {
              res[i].waitForFinished();
            }
          }

          if (qcepDebug(DEBUG_INTEGRATOR)) {
            expt->printMessage(tr("2nd stage complete after %1 msec").arg(tic.elapsed()));
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

        m_Integral.resize(resultSize());
        m_SumValue.resize(resultSize());

        if (m_ThreadCount == 1) {
          partialIntegrationStep3(0, 1, dimg, mask, normalize);
        } else {
          QVector< QFuture<void> > res;

          for (int i=0; i<m_ThreadCount; i++) {
            res.append(QtConcurrent::run(this, &QxrdIntegratorCache::partialIntegrationStep3,
                                         i, m_ThreadCount, dimg, mask, normalize));
          }

          for (int i=0; i<m_ThreadCount; i++) {
            res[i].waitForFinished();
          }
        }

        if (qcepDebug(DEBUG_INTEGRATOR)) {
          expt->printMessage(tr("Stage 3 complete after %1 msec").arg(tic.elapsed()));
        }

        int nRange = 0;

        int nStride = m_NRMax - m_NRMin;

        if (m_HasChi) {
          int ncRange = m_NCMax - m_NCMin;
          nRange = nStride*ncRange;
        } else {
          nRange = nStride;
        }

        double scalingFactor = m_ScalingFactor;

        if (m_SelfNormalization) {
          double nsum = 0, ninteg = 0;

          for(int ir=0; ir<nRange; ir++) {
            int sv = m_SumValue[ir];

            if (sv > 0) {
              double xv = m_RMin + (ir+0.5)*m_RStep;

              if (xv >= m_SelfNormalizationMinimum && xv < m_SelfNormalizationMaximum) {
                nsum   += sv;
                ninteg += m_Integral[ir];
              }
            }
          }

          if ((nsum > 0) && (ninteg != 0)) {
            scalingFactor = m_ScalingFactor * nsum / ninteg;
          }
        }

        if (m_HasChi) {
          QcepDoubleImageDataPtr img = qSharedPointerDynamicCast<QcepDoubleImageData>(res);
          int nRadial = m_NRMax - m_NRMin;
          int nPolar  = m_NCMax - m_NCMin;

          img->resize(nRadial, nPolar);

          for (int y=0; y<nPolar; y++) {
            for (int x=0; x<nRadial; x++) {
              int bin = y*nRadial + x;

              int   sv = m_SumValue[bin];
              double v = m_Integral[bin];

              if (sv > 0) {
                img->setValue(x,y, scalingFactor*normVal*v/sv);
              } else {
                img->setValue(x,y, 0.0/0.0);
              }
            }
          }

          img->dataObjectChanged();
        } else {
          QcepIntegratedDataPtr integ = qSharedPointerDynamicCast<QcepIntegratedData>(res);

          if (integ) {
            integ -> resize(0);
            integ -> set_Center(m_CenterX, m_CenterY);

            for(int ir=0; ir<nRange; ir++) {
              int sv = m_SumValue[ir];

              if (sv > 0) {
                double xv = m_RMin + (ir+0.5)*m_RStep;

                if (normalize) {
                  integ -> append(xv, scalingFactor*normVal*m_Integral[ir]/sv);
                } else {
                  integ -> append(xv, scalingFactor*normVal*m_Integral[ir]/sv*(ir*oversampleStep+halfOversampleStep));
                }
              }
            }

            integ->set_XUnitsLabel(XLabel());
            integ->set_Oversample(m_Oversample);
            // Integrate entirely out of cache
          }
        }
      }

      expt->printMessage(tr("Integration of %1 took %2 msec")
                         .arg(dimg->get_Title())
                         .arg(tic.restart()));
    } else {
      expt->printMessage(tr("QxrdIntegratorCache::performIntegration - integration failed"));
    }
  }
}

void QxrdIntegratorCache::grabScriptEngine()
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QxrdScriptEnginePtr engine = exp->scriptEngine();

    if (engine) {
      engine->lock();

      m_UserGeometryFunctionValue = QScriptValue();
      m_UserAbsorptionFunctionValue = QScriptValue();

      if (m_EnableUserGeometry) {
        engine->evaluate(m_UserGeometryScript);

        m_UserGeometryFunctionValue = engine->evaluate(m_UserGeometryFunction);

        if (!m_UserGeometryFunctionValue.isFunction()) {
          m_UserGeometryFunctionValue = engine->globalObject().property(m_UserGeometryFunctionValue.toString());
        }

        if (m_UserGeometryFunctionValue.isFunction()) {
          exp->printMessage(tr("Using User Geometry Function %1").arg(m_UserGeometryFunctionValue.toString()));
        } else {
          exp->printMessage(tr("User Geometry Function %1 is not a function").arg(m_UserGeometryFunctionValue.toString()));
        }
      }

      if (m_EnableUserAbsorption) {
        engine->evaluate(m_UserAbsorptionScript);

        m_UserAbsorptionFunctionValue = engine->evaluate(m_UserAbsorptionFunction);

        if (!m_UserAbsorptionFunctionValue.isFunction()) {
          m_UserAbsorptionFunctionValue = engine->globalObject().property(m_UserAbsorptionFunctionValue.toString());
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
    QxrdScriptEnginePtr engine = exp->scriptEngine();

    if (engine) {
      engine->unlock();

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
  return m_CachedRadialBinNumbers;
}

QcepDoubleImageDataPtr QxrdIntegratorCache::cachedIntensity()
{
  return m_CachedNormalization;
}
