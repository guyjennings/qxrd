#ifndef QCEPALLOCATORTHREAD_H
#define QCEPALLOCATORTHREAD_H

#include "qceplib_global.h"
#include "qcepthread.h"
#include "qcepapplication-ptr.h"
#include "qcepallocator-ptr.h"

class QCEP_EXPORT QcepAllocatorThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  QcepAllocatorThread(QcepApplicationWPtr   app);
  ~QcepAllocatorThread();

  void shutdown();

  QcepAllocatorPtr allocator() const;

protected:
  void run();

private:
  QcepAllocatorPtr      m_Allocator;
};

#endif // QCEPALLOCATORTHREAD_H
