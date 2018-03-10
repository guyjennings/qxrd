#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrdthread.h"
#include "qxrdsimpleserver-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT

private:
  typedef QxrdThread inherited;

public:
  Q_INVOKABLE QxrdSimpleServerThread(QString name);
  ~QxrdSimpleServerThread();

  void initialize(QcepObjectWPtr parent);
  void shutdown();

  QxrdSimpleServerPtr server() const;

protected:
  void run();

private:
  QxrdExperimentWPtr    m_Experiment;
  QString               m_Name;
  QxrdSimpleServerPtr   m_Server;
};

Q_DECLARE_METATYPE(QxrdSimpleServerThread*)

#endif // QXRDSIMPLESERVERTHREAD_H
