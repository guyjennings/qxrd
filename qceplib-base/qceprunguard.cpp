#include "qceprunguard.h"
#include <QCryptographicHash>

namespace {
QString generateKeyHash( const QString& key, const QString& salt )
{
  QByteArray data;

  data.append( key.toUtf8() );
  data.append( salt.toUtf8() );
  data = QCryptographicHash::hash( data, QCryptographicHash::Sha1 ).toHex();

  return data;
}
}

QcepRunGuard::QcepRunGuard( const QString& key )
  : m_Key( key )
  , m_MemLockKey( generateKeyHash( key, "_memLockKey" ) )
  , m_SharedMemKey( generateKeyHash( key, "_sharedmemKey" ) )
  , m_SharedMem( m_SharedMemKey )
  , m_MemLock( m_MemLockKey, 1 )
{
  m_MemLock.acquire();
  {
    QSharedMemory fix( m_SharedMemKey );    // Fix for *nix: http://habrahabr.ru/post/173281/
    fix.attach();
  }
  m_MemLock.release();
}

QcepRunGuard::~QcepRunGuard()
{
    release();
}

bool QcepRunGuard::isAnotherRunning()
{
    if ( m_SharedMem.isAttached() )
        return false;

    m_MemLock.acquire();
    const bool isRunning = m_SharedMem.attach();
    if ( isRunning )
        m_SharedMem.detach();
    m_MemLock.release();

    return isRunning;
}

bool QcepRunGuard::tryToRun()
{
    if ( isAnotherRunning() )   // Extra check
        return false;

    m_MemLock.acquire();
    const bool result = m_SharedMem.create( sizeof( quint64 ) );
    m_MemLock.release();
    if ( !result )
    {
        release();
        return false;
    }

    return true;
}

void QcepRunGuard::release()
{
    m_MemLock.acquire();
    if ( m_SharedMem.isAttached() )
        m_SharedMem.detach();
    m_MemLock.release();
}
