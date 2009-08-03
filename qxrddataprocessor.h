/******************************************************************
*
*  $Id: qxrddataprocessor.h,v 1.15 2009/08/03 13:26:25 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>

#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"

#include <qwt_double_rect.h>

class QxrdAcquisition;
class QxrdImageData;
class QxrdCenterFinder;
class QxrdIntegrator;

class QxrdDataProcessor : public QObject
{
  Q_OBJECT;

public:
  QxrdDataProcessor(QxrdAcquisition *acq, QObject *parent=0);

public:
  Q_PROPERTY(QString outputDirectory READ get_OutputDirectory WRITE set_OutputDirectory);
  QCEP_STRING_PROPERTY(OutputDirectory);

  Q_PROPERTY(QString darkImagePath   READ get_DarkImagePath WRITE set_DarkImagePath);
  QCEP_STRING_PROPERTY(DarkImagePath);

  Q_PROPERTY(QString badPixelsPath   READ get_BadPixelsPath WRITE set_BadPixelsPath);
  QCEP_STRING_PROPERTY(BadPixelsPath);

  Q_PROPERTY(QString gainMapPath     READ get_GainMapPath WRITE set_GainMapPath);
  QCEP_STRING_PROPERTY(GainMapPath);

  Q_PROPERTY(bool performDarkSubtraction READ get_PerformDarkSubtraction WRITE set_PerformDarkSubtraction);
  QCEP_BOOLEAN_PROPERTY(PerformDarkSubtraction);

  Q_PROPERTY(bool saveRawImages READ get_SaveRawImages WRITE set_SaveRawImages);
  QCEP_BOOLEAN_PROPERTY(SaveRawImages);

  Q_PROPERTY(bool performBadPixels READ get_PerformBadPixels WRITE set_PerformBadPixels);
  QCEP_BOOLEAN_PROPERTY(PerformBadPixels);

  Q_PROPERTY(bool performGainCorrection READ get_PerformGainCorrection WRITE set_PerformGainCorrection);
  QCEP_BOOLEAN_PROPERTY(PerformGainCorrection);

  Q_PROPERTY(QString fileName        READ get_FileName WRITE set_FileName);
  QCEP_STRING_PROPERTY(FileName);

  Q_PROPERTY(double maskMinimumValue READ get_MaskMinimumValue WRITE set_MaskMinimumValue);
  QCEP_DOUBLE_PROPERTY(MaskMinimumValue);

  Q_PROPERTY(double maskMaximumValue READ get_MaskMaximumValue WRITE set_MaskMaximumValue);
  QCEP_DOUBLE_PROPERTY(MaskMaximumValue);

  Q_PROPERTY(double maskCircleRadius READ get_MaskCircleRadius WRITE set_MaskCircleRadius);
  QCEP_DOUBLE_PROPERTY(MaskCircleRadius);

  Q_PROPERTY(bool maskSetPixels READ get_MaskSetPixels WRITE set_MaskSetPixels);
  QCEP_BOOLEAN_PROPERTY(MaskSetPixels);

signals:
  void printMessage(QString msg);
  void newDataAvailable(QxrdImageData *);
  void newDarkImageAvailable(QxrdImageData *);

public slots:
  void showMaskRange(/*double min, double max*/);
  void hideMaskRange(/*double min, double max*/);
  void showMaskAll();
  void hideMaskAll();
  void invertMask();
  void maskCircle(QwtDoubleRect pt);
  void maskPolygon(QwtArray<QwtDoublePoint> poly);

  void loadData(QString name);
  void saveData(QString name);
  void loadDarkImage(QString name);
  void loadBadPixels(QString name);
  void loadGainMap(QString name);

public:
  QxrdImageData *takeNextFreeImage();
//  QxrdImageData *takeLatestProcessedImage();
//  QxrdImageData *takeNextProcessedImage();
//  QxrdImageData *takeNextDarkImage();

  void returnImageToPool(QxrdImageData *img);

  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

  void setAcquisition(QxrdAcquisition *acq);
//  void setWindow(QxrdWindow *win);


  void saveImageData(QxrdImageData *image);
  void saveRawData(QxrdImageData *image);
  void saveNamedImageData(QString name, QxrdImageData *image);

  QxrdImageData *data() const;
  QxrdImageData *darkImage() const;

  int incrementAcquiredCount();
  int decrementAcquiredCount();
  int getAcquiredCount();

  int incrementProcessedCount();
  int decrementProcessedCount();
  int getProcessedCount();

  QxrdCenterFinder  *centerFinder() const;
  QxrdIntegrator    *integrator() const;

private slots:
  void onAcquiredImageAvailable(QxrdImageData *image);
//  void onProcessedImageAvailable(QxrdImageData *image);
//  void onDarkImageAvailable(QxrdImageData *image);

private:
  void processAcquiredImage(QxrdImageData *image);

  void subtractDarkImage(QxrdImageData *image, QxrdImageData *dark);
  void correctBadPixels(QxrdImageData *image);
  void correctImageGains(QxrdImageData *image);
  void performImageCorrections(QxrdImageData *image);

  void newData(QxrdImageData *image);
  void newDarkImage(QxrdImageData *image);
  void newBadPixelsImage(QxrdImageData *image);
  void newGainMapImage(QxrdImageData *image);

private:
  mutable QMutex            m_Mutex;

  QxrdAcquisition          *m_Acquisition;
  QReadWriteLock            m_DarkUsage;
  QReadWriteLock            m_Processing;

  QxrdImageQueue            m_FreeImages;
//  QxrdImageQueue            m_ProcessedImages;
//  QxrdImageQueue            m_DarkImages;

  QxrdImageData            *m_Data;
  QxrdImageData            *m_DarkFrame;
  QxrdImageData            *m_BadPixels;
  QxrdImageData            *m_GainFrame;

  QAtomicInt                m_AcquiredCount;
  QAtomicInt                m_ProcessedCount;

  QxrdCenterFinder         *m_CenterFinder;
  QxrdIntegrator           *m_Integrator;

  HEADER_IDENT("$Id: qxrddataprocessor.h,v 1.15 2009/08/03 13:26:25 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrddataprocessor.h,v $
*  Revision 1.15  2009/08/03 13:26:25  jennings
*  Added option to set/clear mask pixels
*
*  Revision 1.14  2009/08/02 21:14:16  jennings
*  Added masking dummy routines
*
*  Revision 1.13  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.12  2009/07/20 00:31:31  jennings
*  Removed image queues for acquired and dark images - use 'connect' args instead
*
*  Revision 1.11  2009/07/17 20:41:20  jennings
*  Modifications related to mask display
*
*  Revision 1.10  2009/07/17 14:00:59  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.9  2009/07/17 12:41:33  jennings
*  Rearranging acquisition and data processor
*
*  Revision 1.8  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.7  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.6  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

