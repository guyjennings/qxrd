#ifndef QCEPRUNGUARD_H
#define QCEPRUNGUARD_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class QcepRunGuard
{
public:
  QcepRunGuard( const QString& m_Key );
  ~QcepRunGuard();

  bool isAnotherRunning();
  bool tryToRun();
  void release();

private:
  const QString    m_Key;
  const QString    m_MemLockKey;
  const QString    m_SharedMemKey;

  QSharedMemory    m_SharedMem;
  QSystemSemaphore m_MemLock;

  Q_DISABLE_COPY( QcepRunGuard )
};

#endif // QCEPRUNGUARD_H
