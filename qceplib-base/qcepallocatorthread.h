#ifndef QCEPALLOCATORTHREAD_H
#define QCEPALLOCATORTHREAD_H

#include "qcepthread.h"

#include "qcepsettingssaver-ptr.h"
#include "qcepallocator-ptr.h"

class QcepAllocatorThread : public QcepThread
{
  Q_OBJECT

public:
  QcepAllocatorThread(QcepSettingsSaverPtr saver);
  ~QcepAllocatorThread();

  void shutdown();

  QcepAllocatorPtr allocator() const;

protected:
  void run();

private:
  QcepAllocatorPtr     m_Allocator;
  QcepSettingsSaverPtr m_Saver;
};

#endif // QCEPALLOCATORTHREAD_H
