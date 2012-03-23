#ifndef QXRDSIMPLESERVERTHREAD_H
#define QXRDSIMPLESERVERTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include "qxrdsimpleserver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdSimpleServerThread : public QxrdThread
{
  Q_OBJECT
public:
  QxrdSimpleServerThread(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name);
  ~QxrdSimpleServerThread();

  void shutdown();

  QxrdSimpleServerPtr server() const;

protected:
  void run();

private:
  QxrdSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
  QString               m_Name;
  QxrdSimpleServerPtr   m_Server;
};

#endif // QXRDSIMPLESERVERTHREAD_H
