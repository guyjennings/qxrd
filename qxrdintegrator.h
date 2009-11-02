/******************************************************************
*
*  $Id: qxrdintegrator.h,v 1.13 2009/11/02 20:19:27 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include <qwt_double_rect.h>

class QxrdDataProcessor;
//class QxrdImageData;
class QxrdMaskData;

class QxrdIntegrator : public QObject
{
  Q_OBJECT;

public:
  QxrdIntegrator(QxrdDataProcessor *proc, QObject *parent=0);

public:
  Q_PROPERTY(int oversample READ get_Oversample WRITE set_Oversample);
  QCEP_INTEGER_PROPERTY(Oversample);

public:
  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

signals:
  void newIntegrationAvailable(QString title, QVector<double> x, QVector<double> y);
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void integrateSaveAndDisplay();
  void performIntegration();
  void saveIntegratedData();
  void displayIntegratedData();
  void integrate(double cx, double cy, int oversample, int normalize);
  void parallelIntegrate(int nthreads, double cx, double cy, int oversample, int normalize);
  void integrate2(double cx, double cy, int oversample, int normalize);
  void tableIntegrate(int nthreads, double cx, double cy, int oversample, int normalize);

  void sliceLine(double x0, double y0, double x1, double y1, double width);
  void slicePolygon(QwtArray<QwtDoublePoint> poly, double width);

//  void appendToGraph();
//
private:
  void parallelIntegrateMap(int thread, int nthreads, double cx, double cy, int oversample/*, int normalize*/);
  void tableIntegrateSetup(int nthreads, double cx, double cy, int oversample, int normalize);
  void tableIntegrateMap(int thread, int nthreads, double cx, double cy, int oversample/*, int normalize*/);

private:
  mutable QMutex            m_Mutex;
  QxrdDataProcessor        *m_DataProcessor;
  QVector<int>              m_TableData;
  int                       m_TableStride;
  QVector<double>           m_OutputX;
  QVector<double>           m_OutputY;
  QVector<double>           m_OutputData;
  QVector<double>           m_OutputSums;
  int                       m_OutputStride;

  HEADER_IDENT("$Id: qxrdintegrator.h,v 1.13 2009/11/02 20:19:27 jennings Exp $");
};

#endif // QXRDINTEGRATOR_H

/******************************************************************
*
*  $Log: qxrdintegrator.h,v $
*  Revision 1.13  2009/11/02 20:19:27  jennings
*  Changes to make it work with VC compiler
*
*  Revision 1.12  2009/10/05 21:17:46  jennings
*  Integrate button now causes integrated curve to be saved and plotted, as well
*
*  Revision 1.11  2009/10/02 20:11:02  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.10  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
*  Revision 1.9  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.8  2009/08/25 18:43:03  jennings
*  Templatized QxrdImageData and QxrdImageQueue, and added int16, int32 and double variants as typedefs
*
*  Revision 1.7  2009/08/08 20:15:36  jennings
*  Added some more integration routines
*
*  Revision 1.6  2009/08/07 22:21:56  jennings
*  Added a number of sample data creation routines to QxrdDataProcessor
*  Added a parallelized integration routine to QxrdIntegrator
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

