#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qspecserver.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepobject.h"

class QxrdServer : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdServer(QxrdExperimentWPtr doc, QString name);
  virtual ~QxrdServer();

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

signals:
  void executeCommand(QString cmd);
public slots:
  void finishedCommand(QScriptValue result);

public:
  Q_PROPERTY(int    runSpecServer    READ get_RunSpecServer WRITE set_RunSpecServer)
  QCEP_INTEGER_PROPERTY(RunSpecServer)

  Q_PROPERTY(int    specServerPort    READ get_SpecServerPort WRITE set_SpecServerPort)
  QCEP_INTEGER_PROPERTY(SpecServerPort)

public slots:
  void runModeChanged();
  void serverPortChanged();
  void shutdown();

private:
  QSpecServer m_Server;
};

Q_DECLARE_METATYPE(QxrdServer*)

#endif
