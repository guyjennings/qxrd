#ifndef QSPECSERVER_H
#define QSPECSERVER_H

#include "qceplib_global.h"
#include "qcepmacros.h"

#include <QTcpServer>
#include <QHostAddress>
#include <QByteArray>
#include "spec_server.h"
#include <QVector>
#include <QList>
#include <QMetaType>
#include <QScriptValue>
#include <QDateTime>
#include <QTcpSocket>

#include "qobject-ptr.h"
#include "qcepobject-ptr.h"

/**
  QSpecServer implements a remote control interface to the
  'SPEC' data acquisition program.
  */

class QCEP_EXPORT QSpecServer : public QTcpServer
{
  Q_OBJECT

public:
  QSpecServer(QString name);
  void initialize(QObjectWPtr owner);

  virtual ~QSpecServer();

public:
  void startServer(QHostAddress a, int pmin=-1, int pmax=-1);
  void stopServer();
  int  port();

public slots:
  void openNewConnection();
  void connectionClosed();
  void clientRead();

signals:
  void executeCommand(QString cmd);
public slots:
  void finishedCommand(QScriptValue result);

private:
  void reportServerAddress();
  int readPacketData();
  int interpretPacket();
  qint32 swapInt32(qint32 val);
  quint32 swapUInt32(quint32 val);
  qint32 condSwapInt32(qint32 val);
  quint32 condSwapUInt32(quint32 val);
  void init_svr_head(struct svr_head *h);

protected:
  virtual QVariant readProperty(QString name);

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

protected:
  void initReplyPacket();
  void sendReplyPacketHead();
  void sendReplyPacketTail();

  void replyFromVariant(QVariant value);
  void replyFromError(QScriptValue value);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

private:
  QcepObjectWPtr       m_Owner;
  QString              m_ServerName;
  QTcpSocket          *m_Socket;
  QHostAddress         m_Address;
  int                  m_Port;
  int                  m_SwapBytes;
  struct svr_head      m_Packet;
  QByteArray           m_Data;
  struct svr_head      m_Reply;
  QByteArray           m_ReplyData;
  int                  m_ReplyHeadSent;
};

#endif
