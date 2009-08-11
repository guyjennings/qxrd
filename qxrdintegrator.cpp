/******************************************************************
*
*  $Id: qxrdintegrator.cpp,v 1.9 2009/08/11 20:53:42 jennings Exp $
*
*******************************************************************/

#include "qxrdintegrator.h"
#include "qxrddataprocessor.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdcenterfinder.h"

#include <QTime>
#include <QtConcurrentRun>

QxrdIntegrator::QxrdIntegrator(QxrdDataProcessor *proc, QObject *parent)
  : QObject(parent),
    m_Oversample(this, "oversample", 1),
    m_DataProcessor(proc),
    SOURCE_IDENT("$Id: qxrdintegrator.cpp,v 1.9 2009/08/11 20:53:42 jennings Exp $")
{
}

void QxrdIntegrator::writeSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdIntegrator::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdIntegrator::performIntegration()
{
  integrate(m_DataProcessor -> centerFinder() -> get_CenterX(),
            m_DataProcessor -> centerFinder() -> get_CenterY(),
            get_Oversample(), true);
}

void QxrdIntegrator::integrate(double cx, double cy, int oversample, int normalize)
{
  QTime tic;
  tic.start();

  QxrdImageData *image = m_DataProcessor -> data();
  QxrdMaskData  *mask  = m_DataProcessor -> mask();

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
  int irmax = (int) (oversample*sqrt(nRows*nRows+nCols*nCols)) + 2;
  QVector<double> integral(irmax), sumvalue(irmax);

  integral.fill(0);
  sumvalue.fill(0);

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      if (mask->value(x, y)) {
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

  QVector<double> x,y;

  for(int ir=0; ir<irmax; ir++) {
    int sv = sumvalue[ir];

    if (sv > 0) {
      x.append(ir*oversampleStep+halfOversampleStep);
      if (normalize) {
        y.append(integral[ir]/sv);
      } else {
        y.append(integral[ir]/sv*(ir*oversampleStep+halfOversampleStep));
      }
    }
  }

  emit newIntegrationAvailable(x,y);

  emit printMessage(tr("Integration took %1 msec").arg(tic.restart()));
}


void QxrdIntegrator::integrate2(double cx, double cy, int oversample, int normalize)
{
  QTime tic;
  tic.start();

  QxrdImageData *image = m_DataProcessor -> data();
  QxrdMaskData  *mask  = m_DataProcessor -> mask();

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
  int irmax = (int) (oversample*sqrt(nRows*nRows+nCols*nCols)) + 2;
  QVector<double> integral(irmax), sumvalue(irmax);

  integral.fill(0);
  sumvalue.fill(0);

  double *img = image->data();
  bool   *msk = mask ->data();
  double *integ = integral.data();
  double *sumvl = sumvalue.data();

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      if (*msk) {
        double val = *img;
        for (double oversampley = 0; oversampley < 1; oversampley += oversampleStep) {
          double yy = y+oversampley+halfOversampleStep;
          double cy2= (yy - cy)*(yy - cy);
          for (double oversamplex = 0; oversamplex < 1; oversamplex += oversampleStep) {
            double xx = x+oversamplex+halfOversampleStep;
            double r = sqrt(cy2 + (xx-cx)*(xx-cx));
            int ir = (int) (r * oversample);

            if (ir > imax) imax = ir;

            if (ir >= 0 && ir < irmax) {
              integ[ir] += val;
              sumvl[ir] += 1;
            }
          }
        }
      }

      msk++;
      img++;
    }
  }

  QVector<double> x,y;

  for(int ir=0; ir<irmax; ir++) {
    int sv = sumvl[ir];

    if (sv > 0) {
      x.append(ir*oversampleStep+halfOversampleStep);
      if (normalize) {
        y.append(integ[ir]/sv);
      } else {
        y.append(integ[ir]/sv*(ir*oversampleStep+halfOversampleStep));
      }
    }
  }

  emit newIntegrationAvailable(x,y);

  emit printMessage(tr("Integration took %1 msec").arg(tic.restart()));
}

void QxrdIntegrator::parallelIntegrateMap
    (int thread, int nThreads, double cx, double cy, int oversample/*, int normalize*/)
{
  QxrdImageData *image = m_DataProcessor -> data();
  QxrdMaskData  *mask  = m_DataProcessor -> mask();

  int nRows = image -> get_Height();
  int nCols = image -> get_Width();

  if (oversample < 1) {
    oversample = 1;
  } else if (oversample > 16) {
    oversample = 16;
  }

  double oversampleStep = 1.0/oversample;
  double halfOversampleStep = oversampleStep/2.0;

  int imax  = 0;
  int irmax = (int) (oversample*sqrt(nRows*nRows+nCols*nCols)) + 2;
  QVector<double> integral(irmax), sumvalue(irmax);

  integral.fill(0);
  sumvalue.fill(0);

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      if ((x % nThreads) == thread) {
        if (mask->value(x, y)) {
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
  }
}

void QxrdIntegrator::parallelIntegrate(int nThreads, double cx, double cy, int oversample, int normalize)
{
  QTime tic;
  tic.start();

//  parallelIntegrateMap(0,nThreads,cx,cy,oversample/*,normalize*/);

  QVector< QFuture<void> > futures(nThreads);

  for (int i=0; i<nThreads; i++) {
    futures[i] = QtConcurrent::run(this, &QxrdIntegrator::parallelIntegrateMap,
                                   i,nThreads,cx,cy,oversample/*,normalize*/);
  }

  for (int i=0; i<nThreads; i++) {
    futures[i].waitForFinished();
  }

  emit printMessage(tr("Parallel integration took %1 msec with %2 threads").arg(tic.restart()).arg(nThreads));
}

void QxrdIntegrator::tableIntegrateSetup(int nThreads, double cx, double cy, int oversample, int normalize)
{
  QxrdImageData *image = m_DataProcessor -> data();
  QxrdMaskData  *mask  = m_DataProcessor -> mask();

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

//  int imax  = 0;
  int irmax = (int) (oversample*sqrt(nRows*nRows+nCols*nCols)) + 2;
  m_OutputData.resize(nThreads*irmax);
  m_OutputData.fill(0);
  m_OutputSums.resize(nThreads*irmax);
  m_OutputSums.fill(0);
  m_OutputStride = irmax;

//  QVector<double> integral(irmax), sumvalue(irmax);
//
//  integral.fill(0);
//  sumvalue.fill(0);

  m_TableStride = (1+oversample*2);
  m_TableData.resize(nRows*nCols*m_TableStride);
  m_TableData.fill(-1);

  double *img = image->data();
  bool   *msk = mask ->data();
//  double *integ = integral.data();
//  double *sumvl = sumvalue.data();
  int *table = m_TableData.data();

  int nWarnings1 = 0;
  int nWarnings2 = 0;

  int idrmax = m_TableStride+2;
  QVector<int> stats(idrmax+2);
  stats.fill(0);

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      if (*msk) {
        double val = *img;
        int irpxlmin = 10000000;
        int irpxlmax = 0;

        for (double oversampley = 0; oversampley < 1; oversampley += oversampleStep) {
          double yy = y+oversampley+halfOversampleStep;
          double cy2= (yy - cy)*(yy - cy);
          for (double oversamplex = 0; oversamplex < 1; oversamplex += oversampleStep) {
            double xx = x+oversamplex+halfOversampleStep;
            double r = sqrt(cy2 + (xx-cx)*(xx-cx));
            int ir = (int) (r * oversample);

            if (ir < irpxlmin) irpxlmin = ir;
            if (ir > irpxlmax) irpxlmax = ir;
          }
        }

        table[(y*nCols+x)*m_TableStride] = irpxlmin;

        for (double oversampley = 0; oversampley < 1; oversampley += oversampleStep) {
          double yy = y+oversampley+halfOversampleStep;
          double cy2= (yy - cy)*(yy - cy);
          for (double oversamplex = 0; oversamplex < 1; oversamplex += oversampleStep) {
            double xx = x+oversamplex+halfOversampleStep;
            double r = sqrt(cy2 + (xx-cx)*(xx-cx));
            int ir = (int) (r * oversample);

            int idr = ir-irpxlmin;

            if (table[(y*nCols+x)*m_TableStride + 1+idr] < 0) {
              table[(y*nCols+x)*m_TableStride + 1+idr] = 1;
            } else {
              table[(y*nCols+x)*m_TableStride + 1+idr] += 1;
            }

            if (idr < 0) {
              stats[0] += 1;
            } else if (idr < idrmax) {
              stats[idr+1] += 1;
            } else {
              stats[idrmax+1] += 1;
            }
          }
        }

        if ((irpxlmax - irpxlmin) >= m_TableStride) {
          if (nWarnings1++ < 10) {
            printf("Problem at %d,%d, min %d, max %d\n", x,y, irpxlmin, irpxlmax);
          }
        }

        for (int i=0; i<(irpxlmax - irpxlmin); i++) {
          if (table[(y*nCols+x)*m_TableStride + 1+i] < 0) {
            if (nWarnings2++ < 10) {
              printf("Zero at %d,%d,%d\n", x,y, i);
            }
          }
        }
      }

      msk++;
      img++;
    }
  }

  for (int i=0; i < (idrmax+2); i++) {
    printf("%d : %d\n", i-1, stats[i]);
  }
}

void QxrdIntegrator::tableIntegrateMap(int thread, int nThreads, double cx, double cy, int oversample/*, int normalize*/)
{
  QxrdImageData *image = m_DataProcessor -> data();

  int nRows = image -> get_Height();
  int nCols = image -> get_Width();

  if (oversample < 1) {
    oversample = 1;
  } else if (oversample > 16) {
    oversample = 16;
  }

//  double oversampleStep = 1.0/oversample;
//  double halfOversampleStep = oversampleStep/2.0;

//  int irmax = (int) (oversample*sqrt(nRows*nRows+nCols*nCols)) + 2;

  double *img = image->data();
  int *table = m_TableData.data();
  double *output = m_OutputData.data();
  double *outsum = m_OutputSums.data();

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      if (x % nThreads == thread) {
        if (*table != -1) {
          int irmin = *table;
          double val = *img;

          for (int i=1; i<m_TableStride; i++) {
            int nr = table[i];
            if (nr > 0) {
              output[thread*m_OutputStride+irmin+i-1] += nr*val;
              outsum[thread*m_OutputStride+irmin+i-1] += nr;
            } else if (nr == -1) {
              break;
            }
          }
        }
      }

      table += m_TableStride;
      img += 1;
    }
  }
}

void QxrdIntegrator::tableIntegrate(int nThreads, double cx, double cy, int oversample, int normalize)
{
  QTime tic;
  tic.start();

  tableIntegrateSetup(nThreads,cx,cy,oversample,normalize);

  emit printMessage(tr("Table-based integration setup took %1 msec with %2 threads").arg(tic.restart()).arg(nThreads));

  if (oversample < 1) {
    oversample = 1;
  } else if (oversample > 16) {
    oversample = 16;
  }

  double oversampleStep = 1.0/oversample;
  double halfOversampleStep = oversampleStep/2.0;

  QVector< QFuture<void> > futures(nThreads);

  if (nThreads > 1) {
    for (int i=0; i<nThreads; i++) {
      futures[i] = QtConcurrent::run(this, &QxrdIntegrator::tableIntegrateMap,
                                     i,nThreads,cx,cy,oversample/*,normalize*/);
    }

    for (int i=0; i<nThreads; i++) {
      futures[i].waitForFinished();
    }
  } else {
    tableIntegrateMap(0,1,cx,cy,oversample/*,normalize*/);
  }

  double *output = m_OutputData.data();
  double *outsum = m_OutputSums.data();

  for (int i=1; i<nThreads; i++) {
    for (int j=0; j<m_OutputStride; j++) {
      output[j] += output[i*m_OutputStride+j];
      outsum[j] += outsum[i*m_OutputStride+j];
    }
  }

  QVector<double> x,y;

  for(int i=0; i<m_OutputStride; i++) {
    int sv = outsum[i];

    if (sv > 0) {
      x.append(i*oversampleStep+halfOversampleStep);
      if (normalize) {
        y.append(output[i]/sv);
      } else {
        y.append(output[i]/sv*(i*oversampleStep+halfOversampleStep));
      }
    }
  }

  emit newIntegrationAvailable(x,y);

  emit printMessage(tr("Table-based integration took %1 msec with %2 threads").arg(tic.restart()).arg(nThreads));
}

/******************************************************************
*
*  $Log: qxrdintegrator.cpp,v $
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

