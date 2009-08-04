/******************************************************************
*
*  $Id: qxrdintegrator.cpp,v 1.5 2009/08/04 22:03:31 jennings Exp $
*
*******************************************************************/

#include "qxrdintegrator.h"
#include "qxrddataprocessor.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrdcenterfinder.h"

#include <QTime>

QxrdIntegrator::QxrdIntegrator(QxrdDataProcessor *proc, QObject *parent)
  : QObject(parent),
    m_Oversample(this, "oversample", 1),
    m_DataProcessor(proc),
    SOURCE_IDENT("$Id: qxrdintegrator.cpp,v 1.5 2009/08/04 22:03:31 jennings Exp $")
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
  performIntegration(m_DataProcessor -> data(), m_DataProcessor -> mask());
}

void QxrdIntegrator::performIntegration(QxrdImageData *image, QxrdMaskData *mask)
{
  QTime tic;
  tic.start();

  int nRows = image -> get_Height();
  int nCols = image -> get_Width();
  double cx = m_DataProcessor -> centerFinder() -> get_CenterX();
  double cy = m_DataProcessor -> centerFinder() -> get_CenterY();
  int imax  = 0;
  int irmax = (int) sqrt(nRows*nRows+nCols*nCols) + 2;
  int oversample = get_Oversample();
  double oversampleStep = 1.0/oversample;

  QVector<double> integral(irmax*oversample), sumvalue(irmax*oversample);

  integral.fill(0);
  sumvalue.fill(0);

  for (int y=0; y<nRows; y++) {
    for (int x=0; x<nCols; x++) {
      if (mask->value(x, y)) {
        double val = image->value(x, y);
        for (double oversampley = 0; oversampley < 1; oversampley += oversampleStep) {
          double yy = y+oversampley;
          double cy2= (yy - cy)*(yy - cy);
          for (double oversamplex = 0; oversamplex < 1; oversamplex += oversampleStep) {
            double xx = x+oversamplex;
            double r = sqrt(cy2 + (xx-cx)*(xx-cx));
            int ir = (int) (r * oversample);

            if (ir > irmax) irmax = ir;

            integral[ir] += val;
            sumvalue[ir] += 1;
          }
        }
      }
    }
  }

  QVector<double> x,y;

  for(int ir=0; ir<=irmax; ir++) {
    int sv = sumvalue[ir];

    if (sv > 0) {
      x.append(ir*oversampleStep);
      y.append(integral[ir]/sv);
    }
  }

  emit newIntegrationAvailable(x,y);

  emit printMessage(tr("Integration took %1 msec").arg(tic.msec()));
}

/******************************************************************
*
*  $Log: qxrdintegrator.cpp,v $
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

