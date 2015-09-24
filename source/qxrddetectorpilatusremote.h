#ifndef QXRDDETECTORPILATUSREMOTE_H
#define QXRDDETECTORPILATUSREMOTE_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include <QProcess>

class QxrdDetectorPilatusRemote : public QcepObject
{
  Q_OBJECT

public:
  QxrdDetectorPilatusRemote(QcepObject *owner);

signals:

public slots:
  void connectToRemote(QString sshCmd);
  void executeRemote(QString cmd);
  void onReadyRead();

private:
  QProcess m_Process;
};

#endif // QXRDDETECTORPILATUSREMOTE_H
