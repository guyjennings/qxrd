#ifndef QXRDFILESAVERTHREAD_H
#define QXRDFILESAVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"
#include "qxrdallocator.h"
#include "qxrdfilesaver.h"

class QxrdFileSaverThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdFileSaverThread(QxrdAllocatorPtr acq);
  ~QxrdFileSaverThread();

  void shutdown();
  QxrdFileSaverPtr fileSaver() const;

protected:
  void run();

private:
  QxrdFileSaverPtr m_FileSaver;
  QxrdAllocatorPtr m_Allocator;
};

typedef QSharedPointer<QxrdFileSaverThread> QxrdFileSaverThreadPtr;

#endif // QXRDFILESAVERTHREAD_H
