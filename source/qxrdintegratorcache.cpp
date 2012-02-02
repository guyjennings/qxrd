#include "qxrdintegratorcache.h"
#include "qxrdintegrator.h"
#include "qxrddetectorgeometry.h"
#include "qxrdapplication.h"
#include "qcepdebug.h"

#define _USE_MATH_DEFINES

#include <cmath>

QxrdIntegratorCache::QxrdIntegratorCache(QxrdExperiment *exp, QxrdAllocatorPtr alloc) :
  QObject(NULL),
  m_Oversample(NULL, this, "oversample", 1),
  m_IntegrationStep(NULL, this, "integrationStep", 0.001),
  m_IntegrationNSteps(NULL, this, "integrationNSteps", 0),
  m_IntegrationMinimum(NULL, this, "integrationMinimum", 0),
  m_IntegrationMaximum(NULL, this, "integrationMaximum", 100000),
  m_IntegrationXUnits(NULL, this, "integrationXUnits", QxrdIntegrator::IntegrateTTH),
  m_CenterX(NULL, this, "centerX", 0),
  m_CenterY(NULL, this, "centerY", 0),
  m_CenterStep(NULL, this, "centerStep", 1),
  m_DetectorXPixelSize(NULL, this, "detectorXPixelSize", 200),
  m_DetectorYPixelSize(NULL, this, "detectorYPixelSize", 200),
  m_DetectorDistance(NULL, this, "detectorDistance", 1000),
  m_Energy(NULL, this, "energy", 20000),
  m_ImplementTilt(NULL, this,"implementTilt", false),
  m_DetectorTilt(NULL, this, "detectorTilt", 0),
  m_TiltPlaneRotation(NULL, this, "tiltPlaneRotation", 90),
  m_NRows(NULL, this, "nRows", -1),
  m_NCols(NULL, this, "nCols", -1),
  m_RStep(NULL, this, "rStep", 0),
  m_RMin(NULL, this, "rMin", 0),
  m_RMax(NULL, this, "rMax", 0),
  m_NMin(NULL, this, "nMin", 0),
  m_NMax(NULL, this, "nMax", 0),
  m_NRange(NULL, this, "nRange", 0),
  m_Beta(NULL, this, "beta", 0),
  m_CosBeta(NULL, this, "cosBeta", 1),
  m_SinBeta(NULL, this, "sinBeta", 0),
  m_Rot(NULL, this, "rot", 0),
  m_CosRot(NULL, this, "cosRot", 0),
  m_SinRot(NULL, this, "sinRot", 0),
  m_CacheFillLevel(-1),
  m_CacheFullLevel(-1),
  m_Experiment(exp),
  m_Allocator(alloc)
{
}

double QxrdIntegratorCache::getTTH(double x, double y)
{
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {
    return QxrdDetectorGeometry::getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                                             x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                                             cos(beta), sin(beta), cos(rot), sin(rot));
  } else {
    return QxrdDetectorGeometry::getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                                             x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                                             1.0, 0.0, 1.0, 0.0);
  }
}

double QxrdIntegratorCache::getQ(double x, double y)
{
  double q,chi;
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {
    QxrdDetectorGeometry::getQChi(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                                  get_Energy(),
                                  x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                                  rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
                                  &q, &chi);
  } else {
    QxrdDetectorGeometry::getQChi(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                                  get_Energy(),
                                  x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                                  0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                  &q, &chi);
  }

  return q;
}

double QxrdIntegratorCache::getR(double x, double y)
{
  double tth = getTTH(x, y);
  double r = get_DetectorDistance()*tan(tth*M_PI/180.0);

  return r;
}

double QxrdIntegratorCache::XValue(double x, double y)
{
  double xVal = 0;

  switch(get_IntegrationXUnits()) {
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

  return xVal;
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

  switch(get_IntegrationXUnits()) {
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

QxrdIntegratedDataPtr QxrdIntegratorCache::performIntegration(
  QxrdIntegratedDataPtr integ,
  QxrdDoubleImageDataPtr dimg,
  QxrdMaskDataPtr mask,
  int normalize)
{
  QTime tic;
  tic.start();

  if (qcepDebug(DEBUG_INTEGRATOR)) {
    m_Experiment->printMessage(tr("QxrdIntegratorCache::performIntegration"));
  }

  if (integ && dimg) {
    int noversample = get_Oversample();
    double oversampleStep = 1.0/get_Oversample();
    double halfOversampleStep = oversampleStep/2.0;

    QcepDoubleList norm = dimg->get_Normalization();

    double normVal = 1;

    if (norm.length()>=1) {
      normVal = norm[0];
    }

    if (m_CacheFillLevel.testAndSetOrdered(-1,0)) {
      if (qcepDebug(DEBUG_INTEGRATOR)) {
        m_Experiment->printMessage(tr("QxrdIntegratorCache::performIntegration - fill cache"));
      }

      // Allocate new cache and fill it...

      double cx = get_CenterX();
      double cy = get_CenterY();

      int nRows = get_NRows();
      int nCols = get_NCols();
      int nPix  = nRows*nCols*get_Oversample()*get_Oversample();

      double r00  = XValue(0,0);
      double r10  = XValue(nRows+1,0);
      double r01  = XValue(0,nCols+1);
      double r11  = XValue(nRows+1,nCols+1);

      double rMin = r00;
      double rMax = r00;

      rMin = qMin(rMin, r10);
      rMin = qMin(rMin, r01);
      rMin = qMin(rMin, r11);

      rMax = qMax(rMax, r10);
      rMax = qMax(rMax, r01);
      rMax = qMax(rMax, r11);

      if (cx >= 0 && cx <= nCols && cy >= 0 && cy <= nRows) {
        rMin = qMin(rMin,0.0);
      }

      rMin = qMax(rMin, get_IntegrationMinimum());
      rMax = qMin(rMax, get_IntegrationMaximum());

      double rStep = get_IntegrationStep();

      if (rStep == 0) {
        int nStep = get_IntegrationNSteps();

        if (nStep <= 0) {
          nStep = 512;
        }

        rStep = (rMax - rMin)/nStep;
      }

      double nMin  = floor(rMin/rStep);
      double nMax  = ceil(rMax/rStep);
      int nRange = (nMax - nMin);

      set_RStep(rStep);
      set_NRange(nRange);
      set_RMin(rMin);
      set_RMax(rMax);

      m_CachedBinNumbers =  QxrdAllocator::newInt32Image(m_Allocator,
                                                         QxrdAllocator::AlwaysAllocate,
                                                         get_NCols()*get_Oversample(),
                                                         get_NRows()*get_Oversample());

      if (m_CachedBinNumbers) {
        m_CachedBinNumbers -> clear();

        m_CacheFullLevel.fetchAndStoreOrdered(nPix);

        qint32 *cachep = (qint32*) m_CachedBinNumbers->data();

        m_Experiment->commenceWork(nRows);

        for (int y = 0; y < nRows; y++) {
          m_Experiment->completeWork(1);

          for (int x = 0; x < nCols; x++) {
            for (int oversampley = 0; oversampley < noversample; oversampley++) {
              double yy = y+oversampley*oversampleStep+halfOversampleStep;
              for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
                double xx = x+oversamplex*oversampleStep+halfOversampleStep;

                double r = XValue(xx, yy);
                double n = floor(r / rStep);

                if (n >= nMin && n < nMax) {
                  int bin = n - nMin;

                  *cachep++ = bin;
                } else {
                  *cachep++ = -1;
                }

                m_CacheFillLevel.fetchAndAddOrdered(1);
              }
            }
          }
        }

        m_Experiment->finishedWork(nRows);
      }

      if (qcepDebug(DEBUG_INTEGRATOR)) {
        m_Experiment->printMessage(tr("QxrdIntegratorCache::performIntegration - cache finished"));
      }
    }

    while (m_CacheFillLevel < m_CacheFullLevel) {
      if (qcepDebug(DEBUG_INTEGRATOR)) {
        m_Experiment->printMessage(tr("QxrdIntegratorCache::performIntegration - waiting for cache [%1,%2]")
                                    .arg(m_CacheFillLevel)
                                    .arg(m_CacheFullLevel));
      }

      QThreadAccess::msleep(100);
    }

    if (m_CacheFillLevel == m_CacheFullLevel) {
      if (qcepDebug(DEBUG_INTEGRATOR)) {
        m_Experiment->printMessage(tr("QxrdIntegratorCache::performIntegration - use cache"));
      }

      int nRange = get_NRange();
      int nRows = get_NRows();
      int nCols = get_NCols();
      double rMin = get_RMin();
      double rMax = get_RMax();
      double rStep = get_RStep();

      QVector<double> integral(nRange), sumvalue(nRange);
      qint32 *cachep = (qint32*) m_CachedBinNumbers->data();
      int noversample = get_Oversample();

      int nWork = nRows/100;
      int nWorkDone = 0;

      m_Experiment->commenceWork(nWork);

      for (int y = 0; y < nRows; y++) {
        if (y%100 == 0) {
          m_Experiment->completeWork(1);
          nWorkDone++;
        }

        for (int x = 0; x < nCols; x++) {
          if ((mask == NULL) || (mask->value(x, y))) {
            double val = dimg->value(x,y);
            for (int oversampley = 0; oversampley < noversample; oversampley++) {
              for (int oversamplex = 0; oversamplex < noversample; oversamplex++) {
                int bin = *cachep++;

                if (bin >= 0) {
                  integral[bin] += val;
                  sumvalue[bin] += 1;
                }
              }
            }
          } else {
            cachep += (noversample*noversample);
          }
        }
      }

      if (nWorkDone != nWork) {
        m_Experiment->completeWork(nWork-nWorkDone);
      }

      m_Experiment -> finishedWork(nWork);

      integ -> resize(0);
      integ -> set_Center(get_CenterX(), get_CenterY());

      for(int ir=0; ir<nRange; ir++) {
        int sv = sumvalue[ir];

        if (sv > 0) {
          double xv = rMin + (ir+0.5)* /*oversampleStep+halfOversampleStep**/ rStep;

          if (normalize) {
            integ -> append(xv, normVal*integral[ir]/sv);
          } else {
            integ -> append(xv, normVal*integral[ir]/sv*(ir*oversampleStep+halfOversampleStep));
          }
        }
      }

      integ->set_XUnitsLabel(XLabel());
      integ->set_Oversample(get_Oversample());
      // Integrate entirely out of cache
    }

    m_Experiment->printMessage(tr("Integration of %1 took %2 msec")
                                .arg(dimg->get_Title())
                                .arg(tic.restart()));
  } else {
    m_Experiment->printMessage(tr("QxrdIntegratorCache::performIntegration - integration failed"));
  }

  return integ;
}
