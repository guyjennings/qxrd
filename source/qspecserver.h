#ifndef QSPECSERVER_H
#define QSPECSERVER_H

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

#include "qxrdexperiment-ptr.h"

class QSpecServer : public QTcpServer
{
  Q_OBJECT

public:
  QSpecServer(QxrdExperimentWPtr doc, QString name);
  virtual ~QSpecServer();

public:
  void startServer(QHostAddress a, int p=-1);
  void stopServer();

public slots:
  void openNewConnection();
  void connectionClosed();
  void clientRead();

signals:
  void executeCommand(QString cmd);
public slots:
  void finishedCommand(QScriptValue result);

private:
  int readPacketData();
  int interpretPacket();
  qint32 swapInt32(qint32 val);
  quint32 swapUInt32(quint32 val);
  qint32 condSwapInt32(qint32 val);
  quint32 condSwapUInt32(quint32 val);

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
  void sendReplyPacket();
  void replyFromVariant(QVariant value);
  void replyFromError(QScriptValue value);

  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

private:
  QxrdExperimentWPtr   m_Experiment;
  QString              m_ServerName;
//  int                  m_Port;
  QTcpSocket          *m_Socket;
  int                  m_SwapBytes;
  struct svr_head      m_Packet;
  QByteArray           m_Data;
  struct svr_head      m_Reply;
  QByteArray           m_ReplyData;
};

#endif
