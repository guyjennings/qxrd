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
  void interpretLine(QString line);

private:
  QProcess   m_Process;
  QByteArray m_Buffer;
  int        m_FileTransferSize;
  QByteArray m_TransferredFile;
};

#endif // QXRDDETECTORPILATUSREMOTE_H
