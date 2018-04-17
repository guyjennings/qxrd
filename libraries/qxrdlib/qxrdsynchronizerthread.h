#ifndef QXRDSYNCHRONIZERTHREAD_H
#define QXRDSYNCHRONIZERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepthread.h"
#include "qxrdsynchronizerplugin-ptr.h"
#include "qxrdsynchronizer-ptr.h"

class QXRD_EXPORT QxrdSynchronizerThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  Q_INVOKABLE QxrdSynchronizerThread(QString name);
  ~QxrdSynchronizerThread();

  void initialize(QcepObjectWPtr parent);

  QxrdSynchronizerWPtr synchronizer() const;

protected:
  void run();

private:
  QxrdSynchronizerPluginWPtr m_SynchronizerPlugin;
  QxrdSynchronizerPtr        m_Synchronizer;
};

#endif // QXRDSYNCHRONIZERTHREAD_H
