/******************************************************************
*
*  $Id: qxrdintegrator.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include <QObject>
#include "qxrdintegrateddata.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrddataprocessor.h"
#include <qwt_double_rect.h>

class QxrdIntegrator : public QObject
{
  Q_OBJECT;

public:
  QxrdIntegrator(QxrdDataProcessorPtr proc, QxrdAllocatorPtr alloc, QObject *parent=0);

public:
  Q_PROPERTY(int oversample READ get_Oversample WRITE set_Oversample);
  QCEP_INTEGER_PROPERTY(Oversample);

  Q_PROPERTY(double integrationStep READ get_IntegrationStep WRITE set_IntegrationStep);
  QCEP_DOUBLE_PROPERTY(IntegrationStep);

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void integrateSaveAndDisplay();
  QxrdIntegratedDataPtr performIntegration(QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask);
  void saveIntegratedData(QxrdIntegratedDataPtr d);
  void displayIntegratedData(QxrdIntegratedDataPtr d);
  QxrdIntegratedDataPtr integrate(QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, int oversample, int normalize);
  QxrdIntegratedDataPtr integrate(QxrdDoubleImageDataPtr dimg, QxrdMaskDataPtr mask, double cx, double cy, int oversample, int normalize);

  QxrdIntegratedDataPtr sliceLine(QxrdDoubleImageDataPtr dimg, double x0, double y0, double x1, double y1, double width);
  QxrdIntegratedDataPtr slicePolygon(QxrdDoubleImageDataPtr dimg, QwtArray<QwtDoublePoint> poly, double width);

private:
  mutable QMutex       m_Mutex;
  QxrdDataProcessorPtr m_DataProcessor;
  QxrdAllocatorPtr     m_Allocator;

  HEADER_IDENT("$Id: qxrdintegrator.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDINTEGRATOR_H

/******************************************************************
*
*  $Log: qxrdintegrator.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/23 20:47:43  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.13.2.6  2010/06/14 20:57:21  jennings
*  *** empty log message ***
*
*  Revision 1.13.2.5  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.13.2.4  2010/06/11 21:21:07  jennings
*  Added QxrdSharedPointer, QxrdIntegratedData and QxrdIntegratedDataQueue
*  Made integrator more thread-safe
*
*  Revision 1.13.2.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.13.2.2  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.13.2.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
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

