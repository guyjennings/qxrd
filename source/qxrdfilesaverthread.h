#ifndef QXRDFILESAVERTHREAD_H
#define QXRDFILESAVERTHREAD_H

#include "qcepmacros.h"

#include <QThread>
#include "qxrdforwardtypes.h"
#include "qxrdfilesaver.h"

class QxrdFileSaverThread : public QThread
{
  Q_OBJECT;

public:
  QxrdFileSaverThread(QxrdAllocatorPtr acq);
  ~QxrdFileSaverThread();

  void shutdown();

  void setProcessor(QxrdDataProcessorPtr proc);
  void setAcquisition(QxrdAcquisitionPtr acq);

  void saveData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite);
  void saveData(QString name, QxrdMaskDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt32ImageDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt16ImageDataPtr image, int canOverwrite);
  void saveTextData(QString name, QxrdDoubleImageDataPtr image, int canOverwrite);
  void writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data);

signals:
  void printMessage(QDateTime ts, QString msg);
  void statusMessage(QDateTime ts, QString msg);
  void criticalMessage(QDateTime ts, QString msg);

protected:
  void run();

private:
  QxrdFileSaverPtr fileSaver() const;

private:
  QAtomicPointer<QxrdFileSaver>     m_FileSaver;
  QAtomicPointer<QxrdAllocator>     m_Allocator;
};

#endif // QXRDFILESAVERTHREAD_H
