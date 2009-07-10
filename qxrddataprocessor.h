/******************************************************************
*
*  $Id: qxrddataprocessor.h,v 1.7 2009/07/10 22:54:23 jennings Exp $
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
class QxrdWindow;
class QxrdImageData;

class QxrdDataProcessor : public QObject
{
  Q_OBJECT;

public:
  QxrdDataProcessor(QxrdWindow *win, QxrdAcquisition *acq, QObject *parent=0);

signals:
  void processedImageAvailable();
  void darkImageAvailable();
  void printMessage(QString msg);

public:
  QxrdImageData *takeLatestProcessedImage();
  QxrdImageData *takeNextProcessedImage();
  QxrdImageData *takeNextDarkImage();

  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

  void setAcquisition(QxrdAcquisition *acq);
  void setWindow(QxrdWindow *win);

private slots:
  void on_acquired_image_available();

private:
  void processAcquiredImage(QxrdImageData *image);

  void subtractDarkImage(QxrdImageData *image, QxrdImageData *dark);
  void correctBadPixels(QxrdImageData *image);
  void correctImageGains(QxrdImageData *image);
  void performImageCorrections(QxrdImageData *image);

private:
  mutable QMutex            m_Mutex;
  QxrdWindow               *m_Window;
  QxrdAcquisition          *m_Acquisition;
  QReadWriteLock            m_DarkUsage;
  QReadWriteLock            m_Processing;
  QxrdImageQueue            m_ProcessedImages;
  QxrdImageQueue            m_DarkImages;

  HEADER_IDENT("$Id: qxrddataprocessor.h,v 1.7 2009/07/10 22:54:23 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrddataprocessor.h,v $
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

