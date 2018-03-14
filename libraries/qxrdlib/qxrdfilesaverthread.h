#ifndef QXRDFILESAVERTHREAD_H
#define QXRDFILESAVERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qxrdthread.h"
#include "qcepallocator-ptr.h"
#include "qxrdfilesaver-ptr.h"

class QXRD_EXPORT QxrdFileSaverThread : public QxrdThread
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdFileSaverThread(QString name);
  ~QxrdFileSaverThread();

  void shutdown();
  QxrdFileSaverPtr fileSaver() const;

protected:
  void run();

private:
  QxrdFileSaverPtr  m_FileSaver;
};

#endif // QXRDFILESAVERTHREAD_H
