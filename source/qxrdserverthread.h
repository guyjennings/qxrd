#ifndef QXRDSERVERTHREAD_H
#define QXRDSERVERTHREAD_H

#include "qcepmacros.h"

#include "qxrdthread.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdserver-ptr.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdServerThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdServerThread(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name);
  ~QxrdServerThread();

  QxrdServerPtr server() const;

  void shutdown();
  void executeScript(QString cmd);

signals:
  void execute(QString cmd);

protected:
  void run();

private:
  QxrdSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
  QString               m_Name;
  QxrdServerPtr         m_Server;
};

#endif // QXRDSERVERTHREAD_H
