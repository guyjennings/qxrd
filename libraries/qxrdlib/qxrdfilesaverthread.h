#ifndef QXRDFILESAVERTHREAD_H
#define QXRDFILESAVERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qcepthread.h"
#include "qcepallocator-ptr.h"
#include "qxrdfilesaver-ptr.h"

class QXRD_EXPORT QxrdFileSaverThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  Q_INVOKABLE QxrdFileSaverThread(QString name);
  ~QxrdFileSaverThread();

  void haltFileSaver();

  QxrdFileSaverPtr fileSaver() const;

protected:
  void run();

private:
  QxrdFileSaverPtr  m_FileSaver;
};

#endif // QXRDFILESAVERTHREAD_H
