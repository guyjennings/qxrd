#include "qxrdintegrator.h"
#include "qxrddataprocessorbase.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrateddata.h"
#include "qxrdallocator.h"
#include "qxrdapplication.h"

#include <QTime>
#include <QtConcurrentRun>
#include "qxrdmutexlocker.h"

#define _USE_MATH_DEFINES

#include <cmath>

QxrdIntegrator::QxrdIntegrator(QxrdDataProcessorBase *proc, QxrdAllocator *alloc, QObject *parent)
  : QObject(parent),
    m_Oversample(this, "oversample", 1),
    m_IntegrationStep(this, "integrationStep", 0.001),
    m_IntegrationMinimum(this, "integrationMinimum", 0),
    m_IntegrationMaximum(this, "integrationMaximum", INFINITY),
    m_IntegrationXUnits(this, "integrationXUnits", IntegrateTTH),
    m_DataProcessor(proc),
    m_Allocator(alloc)
{
}

QxrdDataProcessorBase *QxrdIntegrator::dataProcessor() const
{
  return m_DataProcessor;
}

void QxrdIntegrator::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdIntegrator::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

QxrdIntegratedDataPtr QxrdIntegrator::performIntegration(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask)
{
  return integrate(integ, dimg, mask, /*m_DataProcessor -> centerFinder() -> get_CenterX(),
                   m_DataProcessor -> centerFinder() -> get_CenterY(),*/
                   get_Oversample(), true);
}

double QxrdIntegrator::XValue(QwtDoublePoint pt) const
{
  return XValue(pt.x(), pt.y());
}

double QxrdIntegrator::XValue(double x, double y) const
{
  double xVal = 0;

  switch(get_IntegrationXUnits()) {
  case IntegrateTTH:
    xVal = m_DataProcessor->centerFinder()->getTTH(x,y);
    break;

  case IntegrateQ:
    xVal = m_DataProcessor->centerFinder()->getQ(x,y);
    break;

  case IntegrateR:
    xVal = m_DataProcessor->centerFinder()->getR(x,y);
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

QxrdIntegratedDataPtr QxrdIntegrator::integrate(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, int oversample, int normalize)
{
  if (integ && image) {
    QcepDoubleList norm = image->get_Normalization();

    double normVal = 1;

    if (norm.length()>=1) {
      normVal = norm[0];
    }

    g_Application->printMessage(tr("Integrating image %1 from %2").arg(image->get_Title()).arg(image->get_FileName()));

    QTime tic;
    tic.start();

    int nRows = image -> get_Height();
    int nCols = image -> get_Width();

    if (oversample < 1) {
      oversample = 1;
      set_Oversample(1);
    } else if (oversample > 16) {
      oversample = 16;
      set_Oversample(16);
    }

    double oversampleStep = 1.0/oversample;
    double halfOversampleStep = oversampleStep/2.0;

    QxrdCenterFinder *cf = m_DataProcessor -> centerFinder();

    if (cf) {
      double cx = cf -> get_CenterX();
      double cy = cf -> get_CenterY();

      double r00  = XValue(QwtDoublePoint(0,0));
      double r10  = XValue(QwtDoublePoint(nRows+1,0));
      double r01  = XValue(QwtDoublePoint(0,nCols+1));
      double r11  = XValue(QwtDoublePoint(nRows+1,nCols+1));

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
      double nMin  = floor(rMin/rStep);
      double nMax  = ceil(rMax/rStep);
      int nRange = (nMax - nMin);
      QVector<double> integral(nRange), sumvalue(nRange);

      double pxl  = cf->get_DetectorXPixelSize();
      double pyl  = cf->get_DetectorYPixelSize();
      double beta = cf->get_DetectorTilt()*M_PI/180.0;
      double rot  = cf->get_TiltPlaneRotation()*M_PI/180.0;
      double dist = cf->get_DetectorDistance();

      if (!cf->get_ImplementTilt()) {
        beta = 0;
        rot = 0;
      }

      double cosbeta = cos(beta);
      double sinbeta = sin(beta);
      double cosrot  = cos(rot);
      double sinrot  = sin(rot);
      int xUnits = get_IntegrationXUnits();

      g_Application->printMessage(tr("Integration range rMin %1, rMax %2, %3 steps").arg(rMin).arg(rMax).arg(nMax - nMin));

      for (int y=0; y<nRows; y++) {
        for (int x=0; x<nCols; x++) {
          if ((mask == NULL) || (mask->value(x, y))) {
            double val = image->value(x, y);
            for (double oversampley = 0; oversampley < 1; oversampley += oversampleStep) {
              double yy = y+oversampley+halfOversampleStep;
              for (double oversamplex = 0; oversamplex < 1; oversamplex += oversampleStep) {
                double xx = x+oversamplex+halfOversampleStep;

//                double r = cf->getTwoTheta(cx,cy,dist,xx,yy,pxl,pyl,cosbeta,sinbeta,cosrot,sinrot);
                double r = XValue(xx,yy);
                double n = floor(r/rStep);

                if (n >= nMin && n < nMax) {
                  int bin = n - nMin;

                  integral[bin] += val;
                  sumvalue[bin] += 1;
                }
              }
            }
          }
        }
      }

      integ -> resize(0);
      integ -> set_Center(cx, cy);

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

      g_Application->printMessage(tr("Integration of %1 took %2 msec").arg(image->get_Title()).arg(tic.restart()));
    } else {
      g_Application->printMessage("QxrdIntegrator::integrate failed");
    }
  }

  return integ;
}

QxrdIntegratedDataPtr QxrdIntegrator::sliceLine(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr image, double x0, double y0, double x1, double y1, double width)
{
  try {
    QwtArray<QwtDoublePoint> poly;
    poly.append(QwtDoublePoint(x0,y0));
    poly.append(QwtDoublePoint(x1,y1));

    return slicePolygon(integ, image, poly, width);
  }

  catch (...) {
    g_Application->printMessage("QxrdIntegrator::sliceLine failed");
  }

  return QxrdIntegratedDataPtr();
}

QxrdIntegratedDataPtr QxrdIntegrator::slicePolygon(QxrdIntegratedDataPtr integ, QxrdDoubleImageDataPtr image, QwtArray<QwtDoublePoint> poly, double /*width*/)
{
  if (integ && image) {
    double length = 0;

    if (poly.size() >= 2) {
      QwtDoublePoint p0 = poly[0];

      for (int i=1; i<poly.size(); i++) {
        QwtDoublePoint p1 = poly[i];
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
        QwtDoublePoint p1 = poly[i];
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
    g_Application->printMessage("QxrdIntegrator::slicePolygon failed");
  }

  return integ;
}
