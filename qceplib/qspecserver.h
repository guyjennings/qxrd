/******************************************************************
*
*  $Id: qspecserver.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

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

class QTcpSocket;

class QSpecServer : public QTcpServer
{
  Q_OBJECT;

public:
  QSpecServer(QString name, int port, QObject *parent=0);

public:
  void startServer(QHostAddress a, int p=-1);

public slots:
  void openNewConnection();
  void connectionClosed();
  void clientRead();

signals:
  void printMessage(QString msg);

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

private:
  QString              m_ServerName;
  int                  m_Port;
  QTcpSocket          *m_Socket;
  int                  m_SwapBytes;
  struct svr_head      m_Packet;
  QByteArray           m_Data;
  struct svr_head      m_Reply;
  QByteArray           m_ReplyData;

  HEADER_IDENT("$Id: qspecserver.h,v 1.2 2010/09/13 20:00:25 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qspecserver.h,v $
*  Revision 1.2  2010/09/13 20:00:25  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.1  2010/04/13 19:29:13  jennings
*  Added qceplib to cvs
*
*  Revision 1.3  2009/06/27 05:05:43  jennings
*  Merged branched versions from qavrg/qspecserver.* and qceplib/qspecserver.*
*
*
*******************************************************************/
