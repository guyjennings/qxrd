#ifndef QXRDFILESAVERTHREAD_H
#define QXRDFILESAVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"
#include "qcepallocator-ptr.h"
#include "qxrdfilesaver-ptr.h"

class QxrdFileSaverThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdFileSaverThread(QcepObjectWPtr parent,
                      QcepAllocatorWPtr acq);
  ~QxrdFileSaverThread();

  void shutdown();
  QxrdFileSaverPtr fileSaver() const;

protected:
  void run();

private:
  QxrdFileSaverPtr  m_FileSaver;
  QcepAllocatorWPtr m_Allocator;
};

#endif // QXRDFILESAVERTHREAD_H
