#ifndef QXRDPILATUSREMOTE_H
#define QXRDPILATUSREMOTE_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include <QProcess>

class QxrdPilatusRemote : public QcepObject
{
  Q_OBJECT

public:
  QxrdPilatusRemote();

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

Q_DECLARE_METATYPE(QxrdPilatusRemote*)

#endif // QXRDPILATUSREMOTE_H
