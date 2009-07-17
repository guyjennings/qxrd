/******************************************************************
*
*  $Id: qxrddataprocessor.h,v 1.9 2009/07/17 12:41:33 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>

#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"

class QxrdAcquisition;
class QxrdImageData;

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

signals:
  void processedImageAvailable(QxrdImageData *);
  void darkImageAvailable(QxrdImageData *);
  void printMessage(QString msg);

public:
  QxrdImageData *takeNextFreeImage();
  QxrdImageData *takeLatestProcessedImage();
  QxrdImageData *takeNextProcessedImage();
  QxrdImageData *takeNextDarkImage();

  void returnImageToPool(QxrdImageData *img);

  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

  void setAcquisition(QxrdAcquisition *acq);
//  void setWindow(QxrdWindow *win);

  void loadData(QString name);
  void saveData(QString name);

  void saveImageData(QxrdImageData *image);
  void saveRawData(QxrdImageData *image);
  void saveNamedImageData(QString name, QxrdImageData *image);

  void loadDarkImage(QString name);
  void loadBadPixels(QString name);
  void loadGainMap(QString name);

  void showMaskRange(double min, double max);
  void hideMaskRange(double min, double max);
  void showMaskAll();
  void hideMaskAll();

  QxrdImageData *data() const;
  QxrdImageData *darkImage() const;

private slots:
  void on_acquired_image_available();

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
  QxrdImageQueue            m_ProcessedImages;
  QxrdImageQueue            m_DarkImages;

  QxrdImageData            *m_Data;
  QxrdImageData            *m_DarkFrame;
  QxrdImageData            *m_BadPixels;
  QxrdImageData            *m_GainFrame;

  HEADER_IDENT("$Id: qxrddataprocessor.h,v 1.9 2009/07/17 12:41:33 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrddataprocessor.h,v $
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

