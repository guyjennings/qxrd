#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qspecserver.h"
#include "qxrdexperiment-ptr.h"
#include "qcepserializableobject.h"

class QxrdServer : public QcepSerializableObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdServer(QString name);
  void initialize(QcepObjectWPtr parent);
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
