#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include <QObject>
#include <QReadWriteLock>

#include "qxrdimagequeue.h"

class QxrdAcquisitionThread;
class QxrdWindow;
class QxrdImageData;

class QxrdDataProcessor : public QObject
{
  Q_OBJECT;

public:
  QxrdDataProcessor(QxrdWindow *win, QxrdAcquisitionThread *acq, QObject *parent=0);

signals:
  void processedImageAvailable();
  void darkImageAvailable();

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
  QxrdAcquisitionThread    *m_AcquisitionThread;
  QReadWriteLock            m_DarkUsage;
  QReadWriteLock            m_Processing;
  QxrdImageQueue            m_ProcessedImages;
  QxrdImageQueue            m_DarkImages;
};

#endif
