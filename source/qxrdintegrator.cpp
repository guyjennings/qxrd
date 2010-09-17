/******************************************************************
*
*  $Id: qxrdintegrator.cpp,v 1.3 2010/09/17 16:23:57 jennings Exp $
*
*******************************************************************/

#include "qxrdintegrator.h"
#include "qxrddataprocessor.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdcenterfinder.h"
#include "qxrdintegrateddata.h"
#include "qxrdallocator.h"

#include <QTime>
#include <QtConcurrentRun>
#include "qxrdmutexlocker.h"

QxrdIntegrator::QxrdIntegrator(QxrdDataProcessorPtr proc, QxrdAllocatorPtr alloc, QObject *parent)
  : QObject(parent),
    m_Oversample(this, "oversample", 1),
    m_IntegrationStep(this, "integrationStep", 0.001),
    m_DataProcessor(proc),
    m_Allocator(alloc),
    SOURCE_IDENT("$Id: qxrdintegrator.cpp,v 1.3 2010/09/17 16:23:57 jennings Exp $")
{
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

void QxrdIntegrator::integrateSaveAndDisplay()
{
  QxrdIntegratedDataPtr integ = performIntegration(m_DataProcessor -> data(), m_DataProcessor -> mask());
  saveIntegratedData(integ);
  displayIntegratedData(integ);
}

QxrdIntegratedDataPtr QxrdIntegrator::performIntegration(QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask)
{
  return integrate(dimg, mask, /*m_DataProcessor -> centerFinder() -> get_CenterX(),
                   m_DataProcessor -> centerFinder() -> get_CenterY(),*/
                   get_Oversample(), true);
}

void QxrdIntegrator::saveIntegratedData(QxrdIntegratedDataPtr data)
{
  m_DataProcessor -> writeOutputScan(data);
}

void QxrdIntegrator::displayIntegratedData(QxrdIntegratedDataPtr data)
{
  m_DataProcessor -> displayIntegratedData(data);
}

QxrdIntegratedDataPtr QxrdIntegrator::integrate(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, int oversample, int normalize)
{
  QxrdIntegratedDataPtr res = m_Allocator -> newIntegratedData(image);

  if (res) {
    emit printMessage(tr("Integrating image %1").arg(image->get_Title()));

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

    QxrdCenterFinderPtr cf = m_DataProcessor -> centerFinder();

    if (cf) {
      double cx = cf -> get_CenterX();
      double cy = cf -> get_CenterY();

      double rMin = cf ->getTTH(QwtDoublePoint(0,0));
      double rMax = rMin;

      double r10  = cf->getTTH(QwtDoublePoint(nRows+1,0));
      double r01  = cf->getTTH(QwtDoublePoint(0,nCols+1));
      double r11  = cf->getTTH(QwtDoublePoint(nRows+1,nCols+1));

      rMin = qMin(rMin, r10);
      rMin = qMin(rMin, r01);
      rMin = qMin(rMin, r11);

      rMax = qMax(rMax, r10);
      rMax = qMax(rMax, r01);
      rMax = qMax(rMax, r11);

      if (cx >= 0 && cx <= nCols && cy >= 0 && cy <= nRows) {
        rMin = qMin(rMin,0.0);
      }

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

      emit printMessage(tr("Integration range rMin %1, rMax %2, %3 steps").arg(rMin).arg(rMax).arg(nMax - nMin));

      for (int y=0; y<nRows; y++) {
        for (int x=0; x<nCols; x++) {
          if ((mask == NULL) || (mask->value(x, y))) {
            double val = image->value(x, y);
            for (double oversampley = 0; oversampley < 1; oversampley += oversampleStep) {
              double yy = y+oversampley+halfOversampleStep;
              for (double oversamplex = 0; oversamplex < 1; oversamplex += oversampleStep) {
                double xx = x+oversamplex+halfOversampleStep;

                double r = cf->getTwoTheta(cx,cy,dist,xx,yy,pxl,pyl,cosbeta,sinbeta,cosrot,sinrot);
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

      res -> resize(0);
      res -> set_Center(cx, cy);

      for(int ir=0; ir<nRange; ir++) {
        int sv = sumvalue[ir];

        if (sv > 0) {
          double xv = (ir+0.5)*/*oversampleStep+halfOversampleStep**/rStep;

          if (normalize) {
            res -> append(xv, integral[ir]/sv);
          } else {
            res -> append(xv, integral[ir]/sv*(ir*oversampleStep+halfOversampleStep));
          }
        }
      }
      //
      //  emit newIntegrationAvailable(image->get_Title(), x,y);

      emit printMessage(tr("Integration of %1 took %2 msec").arg(image->get_Title()).arg(tic.restart()));
    } else {
      printf("QxrdIntegrator::integrate failed\n");
    }
  }

  return res;
}

QxrdIntegratedDataPtr QxrdIntegrator::integrate(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr mask, double cx, double cy, int oversample, int normalize)
{
  QxrdIntegratedDataPtr res = m_Allocator -> newIntegratedData(image);

  if (res) {
    emit printMessage(tr("Integrating image %1").arg(image->get_Title()));

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

    int imax  = 0;
    int irmax = (int) (oversample*sqrt((double)(nRows*nRows+nCols*nCols))) + 2;
    QVector<double> integral(irmax), sumvalue(irmax);

    integral.fill(0);
    sumvalue.fill(0);

    for (int y=0; y<nRows; y++) {
      for (int x=0; x<nCols; x++) {
        if ((mask == NULL) || (mask->value(x, y))) {
          double val = image->value(x, y);
          for (double oversampley = 0; oversampley < 1; oversampley += oversampleStep) {
            double yy = y+oversampley+halfOversampleStep;
            double cy2= (yy - cy)*(yy - cy);
            for (double oversamplex = 0; oversamplex < 1; oversamplex += oversampleStep) {
              double xx = x+oversamplex+halfOversampleStep;
              double r = sqrt(cy2 + (xx-cx)*(xx-cx));
              int ir = (int) (r * oversample);

              if (ir > imax) imax = ir;

              if (ir >= 0 && ir < irmax) {
                integral[ir] += val;
                sumvalue[ir] += 1;
              }
            }
          }
        }
      }
    }

    //  QVector<double> x,y;

    res -> resize(0);
    res -> set_Center(cx, cy);

    for(int ir=0; ir<irmax; ir++) {
      int sv = sumvalue[ir];

      if (sv > 0) {
        double xv = ir*oversampleStep+halfOversampleStep;

        if (normalize) {
          res -> append(xv, integral[ir]/sv);
        } else {
          res -> append(xv, integral[ir]/sv*(ir*oversampleStep+halfOversampleStep));
        }
      }
    }
    //
    //  emit newIntegrationAvailable(image->get_Title(), x,y);

    emit printMessage(tr("Integration of %1 took %2 msec").arg(image->get_Title()).arg(tic.restart()));
  } else {
    printf("QxrdIntegrator::integrate failed\n");
  }

  return res;
}

QxrdIntegratedDataPtr QxrdIntegrator::sliceLine(QxrdDoubleImageDataPtr image, double x0, double y0, double x1, double y1, double width)
{
  try {
    QwtArray<QwtDoublePoint> poly;
    poly.append(QwtDoublePoint(x0,y0));
    poly.append(QwtDoublePoint(x1,y1));

    return slicePolygon(image, poly, width);
  }

  catch (...) {
    printf("QxrdIntegrator::sliceLine failed\n");
  }
}

QxrdIntegratedDataPtr QxrdIntegrator::slicePolygon(QxrdDoubleImageDataPtr image, QwtArray<QwtDoublePoint> poly, double /*width*/)
{
  QxrdIntegratedDataPtr res = m_Allocator -> newIntegratedData(image);

  if (res) {
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
      res -> resize(0);

      for (int i=1; i<poly.size(); i++) {
        QwtDoublePoint p1 = poly[i];
        double dx = p1.x() - p0.x();
        double dy = p1.y() - p0.y();
        double len = sqrt(dx*dx + dy*dy);

        if (len > 0) {
          for (; r<len; r+=1) {
            double x = p0.x() + r*dx/len;
            double y = p0.y() + r*dy/len;

            res -> append(r+r0, image->value((int) x, (int) y));
          }
        }

        p0 = p1;
        r0 += len;
        r  -= len;
      }
      //
      //    emit newIntegrationAvailable(image->get_Title(),xs,ys);

      saveIntegratedData(res);
      displayIntegratedData(res);
    }
  } else {
    printf("QxrdIntegrator::slicePolygon failed\n");
  }

  return res;
}

/******************************************************************
*
*  $Log: qxrdintegrator.cpp,v $
*  Revision 1.3  2010/09/17 16:23:57  jennings
*  Made integrator algorithm honor the 'implementTilt' parameter
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.18.2.6  2010/07/21 19:39:31  jennings
*  Removed some commented out code
*
*  Revision 1.18.2.5  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.18.2.4  2010/06/11 21:21:07  jennings
*  Added QxrdSharedPointer, QxrdIntegratedData and QxrdIntegratedDataQueue
*  Made integrator more thread-safe
*
*  Revision 1.18.2.3  2010/05/25 20:43:37  jennings
*  Added try catch blocks around various memory allocations
*
*  Revision 1.18.2.2  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.18.2.1  2010/04/21 21:45:03  jennings
*  Changed mask values to shorts instead of bools.  Added ROI and histogram calculation tests.
*
*  Revision 1.18  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.17  2009/11/02 20:19:27  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.16  2009/10/21 20:31:55  jennings
*  Slices are automatically saved and plotted.
*
*  Revision 1.15  2009/10/05 21:17:46  jennings
*  Integrate button now causes integrated curve to be saved and plotted, as well
*
*  Revision 1.14  2009/10/02 20:11:02  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.13  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
*  Revision 1.12  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.11  2009/09/07 22:10:14  jennings
*  Allow NULL mask
*
*  Revision 1.10  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.9  2009/08/11 20:53:42  jennings
*  Added automatic plot style options to plot curves
*
*  Revision 1.8  2009/08/08 20:15:36  jennings
*  Added some more integration routines
*
*  Revision 1.7  2009/08/07 22:21:56  jennings
*  Added a number of sample data creation routines to QxrdDataProcessor
*  Added a parallelized integration routine to QxrdIntegrator
*
*  Revision 1.6  2009/08/05 16:44:08  jennings
*  More changes to oversampling code for integration
*
*  Revision 1.5  2009/08/04 22:03:31  jennings
*  Moved integration code into QxrdIntegrator, added oversampling option
*  Add each integration result to the az-avg plot panel
*
*  Revision 1.4  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.3  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

