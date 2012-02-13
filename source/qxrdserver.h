#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qcepmacros.h"

#include "qspecserver.h"
#include <QVariant>

class QxrdServer : public QSpecServer
{
  Q_OBJECT

public:
  QxrdServer(QxrdExperiment *doc, QString name, int port);
  virtual ~QxrdServer();

public slots:
  QVariant executeCommand(QString cmd);
  QVariant readProperty(QString name);
  void shutdown();
};

typedef QSharedPointer<QxrdServer> QxrdServerPtr;

#endif
