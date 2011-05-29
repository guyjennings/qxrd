#ifndef QXRDFILESAVERTHREAD_H
#define QXRDFILESAVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"

class QxrdAllocator;
class QxrdDataProcessor;
class QxrdAcquisition;
class QxrdFileSaver;

#include "qxrdimagedata.h"
#include "qxrdintegrateddata.h"

class QxrdFileSaverThread : public QxrdThread
{
  Q_OBJECT;

public:
  QxrdFileSaverThread(QxrdAllocator *acq);
  ~QxrdFileSaverThread();

  void shutdown();

  void setProcessor(QxrdDataProcessor *proc);
  void setAcquisition(QxrdAcquisition *acq);

  void saveData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveData(QString name, QxrdMaskDataPtr image, int canOverwrite);
  void saveRawData(QString name, QxrdInt32ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveRawData(QString name, QxrdInt16ImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void saveTextData(QString name, QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow, int canOverwrite);
  void writeOutputScan(FILE* logFile, QxrdIntegratedDataPtr data);
  void writeOutputScan(QString dir, QxrdIntegratedDataPtr data);

protected:
  void run();

private:
  QxrdFileSaver *fileSaver() const;

private:
  QAtomicPointer<QxrdFileSaver>     m_FileSaver;
  QAtomicPointer<QxrdAllocator>     m_Allocator;
};

#endif // QXRDFILESAVERTHREAD_H
