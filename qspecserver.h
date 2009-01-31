#ifndef QSPECSERVER_H
#define QSPECSERVER_H

#include <QTcpServer>
#include <QHostAddress>
#include <QByteArray>
#include "spec_server.h"

class QTcpSocket;

class QSpecServer : public QTcpServer
{
  Q_OBJECT;

 public:
  QSpecServer(QString name, QObject *parent=0);

 public:
  void startServer(QHostAddress a, int p=-1);

 public slots:
  void openNewConnection();
  void connectionClosed();
  void clientRead();

 signals:
  void print_message(QString msg);

 private:
  int readPacketData();
  int interpretPacket();
  qint32 swapInt32(qint32 val);

 protected:
  virtual void handle_abort();
  virtual void handle_cmd();
  virtual void handle_cmd_return();
  virtual void handle_register();
  virtual void handle_unregister();
  virtual void handle_func();
  virtual void handle_func_return();
  virtual void handle_read();
  virtual void handle_send();
  virtual void handle_hello();

  virtual QVariant executeCommand(QString cmd);
  virtual QVariant readProperty(QString name);

 protected:
  void initReplyPacket();
  void sendReplyPacket();
  void replyFromVariant(QVariant value);

 private:
  QString              m_ServerName;
  QTcpSocket          *m_Socket;
  int                  m_SwapBytes;
  struct svr_head      m_Packet;
  QByteArray           m_Data;
  struct svr_head      m_Reply;
  QByteArray           m_ReplyData;
};

#endif
