#ifndef QSPECREMOTE_H
#define QSPECREMOTE_H

#include "qceplib_global.h"
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QVariant>

struct svr_head;

class QCEP_EXPORT QSpecRemote : public QObject
{
  Q_OBJECT

public:
  QSpecRemote(QObject *parent=NULL, const char *name=0);
  virtual ~QSpecRemote();
  int close();
  int abort();

  int connectToHost (const QString &host, const QString &portname=QString());
  int sendHello();
  QVariant getProperty(const QString &prop);
  void     watchProperty(const QString &prop);
  void     setProperty(const QString &prop, const QVariant &value);
  void     watchMotor(const QString &motor);
  void     sendCmd(const QString &cmd);
  QVariant sendCmdReply(const QString &cmd);

private:
  void init();
  QVariant getReturnValue();
  void     dispatchPacket();

  void connectReadyRead();
  void disconnectReadyRead();

private slots:
 void hostFound();
 void connected();
 void disconnected();
 void readyRead();
 void bytesWritten(qint64);
 void error( QAbstractSocket::SocketError );

signals:
  void socketStateChanged(int state);
  void statusMessage(const QString &msg);

private:
  unsigned         m_SerialNumber;
  bool             m_ExpectHeader;
  QTcpSocket      *m_SpecSocket;
  struct svr_head *m_Packet;
  QByteArray       m_Data;
};

#endif // QSPECREMOTE_H
