#ifndef QXRDDETECTORPILATUSREMOTE_H
#define QXRDDETECTORPILATUSREMOTE_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include <QProcess>

class QXRD_EXPORT QxrdDetectorPilatusRemote : public QcepObject
{
  Q_OBJECT

public:
  QxrdDetectorPilatusRemote();

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
