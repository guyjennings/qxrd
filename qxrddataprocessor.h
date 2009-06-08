#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include <QObject>
#include <QReadWriteLock>

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

private slots:
  void on_acquired_image_available();

private:
  void processAcquiredImage(QxrdImageData *image);

  void subtractDarkImage(QxrdImageData *image, QxrdImageData *dark);
  void correctBadPixels(QxrdImageData *image);
  void correctImageGains(QxrdImageData *image);
  void performImageCorrections(QxrdImageData *image);

private:
  QxrdWindow               *m_Window;
  QxrdAcquisition          *m_Acquisition;
  QReadWriteLock            m_DarkUsage;
  QReadWriteLock            m_Processing;
  QxrdImageQueue            m_ProcessedImages;
  QxrdImageQueue            m_DarkImages;
};

#endif
