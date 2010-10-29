#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qspecserver.h"
#include <QVariant>

class QxrdServer : public QSpecServer
{
  Q_OBJECT;

public:
  QxrdServer(QxrdAcquisitionThreadPtr acqth, QString name, int port, QObject *parent=0);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  QVariant executeCommand(QString cmd);
  QVariant readProperty(QString name);
  void shutdown();

private:
  QxrdAcquisitionThreadPtr m_AcquisitionThread;
};

#endif
