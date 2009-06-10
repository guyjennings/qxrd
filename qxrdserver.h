#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qspecserver.h"

class QxrdApplication;
class QxrdAcquisitionThread;

#include <QScriptEngine>

class QxrdServer : public QSpecServer
{
  Q_OBJECT;

public:
  QxrdServer(QxrdAcquisitionThread *acqth, QString name, QObject *parent=0);

signals:
  void printMessage(QString msg);

public slots:
  QVariant executeCommand(QString cmd);
  QVariant readProperty(QString name);
  void shutdown();

private:
  QxrdAcquisitionThread    *m_AcquisitionThread;
};

#endif
