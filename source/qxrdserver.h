#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qcepmacros.h"
#include "qcepproperty.h"
#include "qspecserver.h"

class QxrdServer : public QSpecServer
{
  Q_OBJECT

public:
  QxrdServer(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr doc, QString name);
  virtual ~QxrdServer();

  virtual void readSettings(QSettings *settings, QString section);
  virtual void writeSettings(QSettings *settings, QString section);

public:
  Q_PROPERTY(int    runSpecServer    READ get_RunSpecServer WRITE set_RunSpecServer)
  QCEP_INTEGER_PROPERTY(RunSpecServer)

  Q_PROPERTY(int    specServerPort    READ get_SpecServerPort WRITE set_SpecServerPort)
  QCEP_INTEGER_PROPERTY(SpecServerPort)

public slots:
  void runModeChanged();
  void serverPortChanged();
  QVariant executeCommand(QString cmd);
  QVariant readProperty(QString name);
  void shutdown();
};

#endif
