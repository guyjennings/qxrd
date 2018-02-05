#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qspecserver.h"
#include "qxrdexperiment-ptr.h"
#include "qcepobject.h"

//TODO: need separate settings class for experiment thread
class QXRD_EXPORT QxrdServer : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdServer(QString name);
  void initialize(QObjectWPtr parent);
  virtual ~QxrdServer();

  virtual void readSettings(QSettings *settings);
  virtual void writeSettings(QSettings *settings);

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
