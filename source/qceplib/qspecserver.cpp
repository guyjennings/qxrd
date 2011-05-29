#include "qspecserver.h"

#include <QTcpSocket>
#include <QTextStream>
#include <QVariant>
#include <QDateTime>
#include <QTime>

#include "qxrdapplication.h"

QSpecServer::QSpecServer(QString name, int port, QObject *parent)
  : QTcpServer(parent),
    m_ServerName(name),
    m_Port(port),
    m_Socket(NULL)
{
  connect(this, SIGNAL(newConnection()), this, SLOT(openNewConnection()));
}

void
QSpecServer::startServer(QHostAddress a, int p)
{
  setMaxPendingConnections(1);

  if (isListening()) {
    close();
  }

  if (p < 0) {
    for (int p=6510; p<=6530; p++) {
      if (listen(a,p)) {
        g_Application->printMessage(tr("Bound to port %1").arg(p));
	return;
      }
    }
    qWarning("Couldn't find unused port in range [6510..6530] to use for spec server");
  } else {
    if (!listen(a, p)) {
      qWarning("Failed to bind to address %s port %d",
	       qPrintable(a.toString()), p);
    }
  }
}

void
QSpecServer::openNewConnection()
{
  m_Socket = nextPendingConnection();

  connect(m_Socket, SIGNAL(disconnected()),
	  m_Socket, SLOT(deleteLater()));

  connect(m_Socket, SIGNAL(readyRead()),
	  this,     SLOT(clientRead()));

  g_Application->printMessage(QString("New connection from %1")
		     .arg(m_Socket->peerAddress().toString()) );

  connect(m_Socket, SIGNAL(disconnected()),
	  this,     SLOT(connectionClosed()));
}

void
QSpecServer::connectionClosed()
{
  g_Application->printMessage("Client closed connection");
}

void
QSpecServer::clientRead()
{
  quint64 avail = m_Socket -> bytesAvailable();

//   g_Application->printMessage(tr("QSpecServer::clientRead, %1 bytes available").arg(avail));

  if (avail >= sizeof(struct svr_head)) {
    m_Socket -> read((char*) &m_Packet, sizeof(struct svr_head));
    m_Data.resize(0);

    avail -= sizeof(struct svr_head);

//     g_Application->printMessage(tr("QSpecServer::clientRead, %1 bytes available").arg(avail));

    readPacketData();
  }
}

int
QSpecServer::readPacketData()
{
  if (m_Packet.magic == SV_SPEC_MAGIC) {
    m_SwapBytes = 0;
  } else if (m_Packet.magic == swapUInt32(SV_SPEC_MAGIC)) {
    m_SwapBytes = 1;
  } else {
    g_Application->printMessage(tr("Bad packet\n"));
    return 0;
  }

  if (m_SwapBytes) {
    m_Packet.magic = swapInt32(m_Packet.magic);
    m_Packet.vers  = swapInt32(m_Packet.vers);

    m_Packet.size  = swapInt32(m_Packet.size);
    m_Packet.sn    = swapInt32(m_Packet.sn);

    m_Packet.sec   = swapInt32(m_Packet.sec);
    m_Packet.usec  = swapInt32(m_Packet.usec);

    m_Packet.cmd   = swapInt32(m_Packet.cmd);
    m_Packet.type  = swapInt32(m_Packet.type);
    m_Packet.rows  = swapInt32(m_Packet.rows);
    m_Packet.cols  = swapInt32(m_Packet.cols);
    m_Packet.len   = swapInt32(m_Packet.len);
    m_Packet.err   = swapInt32(m_Packet.err);
    m_Packet.flags = swapInt32(m_Packet.flags);
  }

  if (m_Packet.len > 0) {

    if (m_Packet.len > m_Socket->bytesAvailable()) {
      m_Socket -> waitForReadyRead();
    }

    m_Data.append(m_Socket->read(m_Packet.len));
  }

  return interpretPacket();
}

int
QSpecServer::interpretPacket()
{
//   g_Application->printMessage(tr("Packet received: magic %1, version %2, size %3")
// 		     .arg(m_Packet.magic).arg(m_Packet.vers).arg(m_Packet.size));
//   g_Application->printMessage(tr("Packet sn %1, sec %2, usec %3")
// 		     .arg(m_Packet.sn).arg(m_Packet.sec).arg(m_Packet.usec));
//   g_Application->printMessage(tr("Packet cmd %1, type %2, rows %3, cols %4, len %5")
// 		     .arg(m_Packet.cmd).arg(m_Packet.type).arg(m_Packet.rows).arg(m_Packet.cols).arg(m_Packet.len));

  switch (m_Packet.cmd) {
  case SV_CLOSE:
//    g_Application->printMessage(tr("SV_CLOSE"));
    break;

  case SV_ABORT:
    handle_abort();
    break;

  case SV_CMD:
    handle_cmd();
    break;

  case SV_CMD_WITH_RETURN:
    handle_cmd_return();
    break;

  case SV_RETURN:
//    g_Application->printMessage(tr("SV_RETURN"));
    break;

  case SV_REGISTER:
    handle_register();
    break;

  case SV_UNREGISTER:
    handle_unregister();
    break;

  case SV_EVENT:
//    g_Application->printMessage(tr("SV_EVENT"));
    break;

  case SV_FUNC:
    handle_func();
    break;

  case SV_FUNC_WITH_RETURN:
    handle_func_return();
    break;

  case SV_CHAN_READ:
    handle_read();
    break;

  case SV_CHAN_SEND:
    handle_send();
    break;

  case SV_REPLY:
//    g_Application->printMessage(tr("SV_REPLY"));
    break;

  case SV_HELLO:
    handle_hello();
    break;

  case SV_HELLO_REPLY:
//    g_Application->printMessage(tr("SV_HELLO_REPLY"));
    break;

  default:
//    g_Application->printMessage(tr("default"));
    break;
  }

  return 0;
}

void
QSpecServer::initReplyPacket()
{
  memset(&m_Reply, 0, sizeof(m_Reply));

  m_Reply.magic = condSwapInt32(SV_SPEC_MAGIC);
  m_Reply.vers  = condSwapInt32(4);
  m_Reply.size  = condSwapInt32(sizeof(m_Reply));
  m_Reply.sn    = condSwapInt32(m_Packet.sn);

  QDateTime now = QDateTime::currentDateTime();

  m_Reply.sec   = condSwapInt32(now.toTime_t());
  m_Reply.usec  = condSwapInt32(now.time().msec()*1000);

  m_Reply.cmd   = 0;
  m_Reply.type  = condSwapInt32(SV_STRING);
  m_Reply.rows  = 0;
  m_Reply.cols  = 0;
  m_Reply.len   = 0;
  m_Reply.err   = 0;
  m_Reply.flags = 0;

  m_ReplyData.resize(0);
}

void
QSpecServer::sendReplyPacket()
{
//  if (m_SwapBytes) {
//    printf("Byte swapping not yet implemented for replies\n");
//    return;
//  }

//   printf("sendReplyPacket %d + %d\n", sizeof(m_Reply), m_ReplyData.size());

  m_Socket -> write((char*) &m_Reply, sizeof(m_Reply));
  m_Socket -> write(m_ReplyData);
  m_Socket -> flush();
}

qint32
QSpecServer::swapInt32(qint32 val)
{
  union {
    qint32 res;
    char buf[4];
  };
  
  char tmp;

  res = val;

  tmp = buf[0]; buf[0] = buf[3]; buf[3] = tmp;
  tmp = buf[1]; buf[1] = buf[2]; buf[2] = tmp;

  return res;
}

quint32
QSpecServer::swapUInt32(quint32 val)
{
  union {
    quint32 res;
    char buf[4];
  };

  char tmp;

  res = val;

  tmp = buf[0]; buf[0] = buf[3]; buf[3] = tmp;
  tmp = buf[1]; buf[1] = buf[2]; buf[2] = tmp;

  return res;
}

qint32 QSpecServer::condSwapInt32(qint32 val)
{
  if (m_SwapBytes) {
    return swapInt32(val);
  } else {
    return val;
  }
}

quint32 QSpecServer::condSwapUInt32(quint32 val)
{
  if (m_SwapBytes) {
    return swapUInt32(val);
  } else {
    return val;
  }
}

void QSpecServer::handle_abort()
{
//  g_Application->printMessage(tr("SV_ABORT"));
}

void QSpecServer::handle_cmd()
{
//  g_Application->printMessage(tr("SV_CMD: %1").arg(QString(m_Data)));

  emit executeCommand(QString(m_Data));
}

void QSpecServer::handle_cmd_return()
{
//  g_Application->printMessage(tr("SV_CMD_WITH_RETURN: %1").arg(QString(m_Data)));
  emit executeCommand(QString(m_Data));
}

void QSpecServer::finishedCommand(QScriptValue result)
{
  if (result.isError()) {
    replyFromError(result);
  } else {
    replyFromVariant(result.toVariant());
  }
}

void QSpecServer::handle_register()
{
//  g_Application->printMessage(tr("SV_REGISTER: %1").arg(m_Packet.name));

  initReplyPacket();
  m_Reply.cmd = condSwapInt32(SV_EVENT);
  m_Reply.type= condSwapInt32(SV_STRING);

  strcpy((char*) &m_Reply.name, m_Packet.name);
  m_ReplyData.append("0");
  m_ReplyData.append('\0');
  m_Reply.len = condSwapInt32(m_ReplyData.size());

  sendReplyPacket();
}

void QSpecServer::handle_unregister()
{
//  g_Application->printMessage(tr("SV_UNREGISTER: %1").arg(m_Packet.name));
}

void QSpecServer::handle_func()
{
//  g_Application->printMessage(tr("SV_FUNC: %1").arg(m_Packet.name));
}

void QSpecServer::handle_func_return()
{
//  g_Application->printMessage(tr("SV_FUNC_WITH_RETURN: %1").arg(m_Packet.name));

  initReplyPacket();
  m_Reply.cmd = condSwapInt32(SV_REPLY);
  m_Reply.type= condSwapInt32(SV_STRING);

  strcpy((char*) &m_Reply.name, m_Packet.name);
  m_ReplyData.append("0");
  m_ReplyData.append('\0');
  m_Reply.len = condSwapInt32(m_ReplyData.size());

  sendReplyPacket();  
}

void QSpecServer::handle_read()
{
//  g_Application->printMessage(tr("SV_CHAN_READ: %1").arg(m_Packet.name));

  QVariant res = readProperty( m_Packet.name);

  replyFromVariant(res);
}

void QSpecServer::handle_send()
{
//  g_Application->printMessage(tr("SV_CHAN_SEND: %1").arg(m_Packet.name));
}

void QSpecServer::handle_hello()
{
//  g_Application->printMessage(tr("SV_HELLO: %1").arg(m_Packet.name));

  initReplyPacket();

  m_Reply.cmd = condSwapInt32(SV_HELLO_REPLY);
  m_Reply.type= condSwapInt32(SV_STRING);

  strcpy((char*) &m_Reply.name, qPrintable(m_ServerName));
  m_ReplyData.append(m_ServerName);
  m_ReplyData.append('\0');
  m_Reply.len = condSwapInt32(m_ReplyData.size());

  sendReplyPacket();
}

void QSpecServer::replyFromVariant(QVariant value)
{
//  g_Application->printMessage(tr("replyFromVariant(%1)").arg(value.typeName()));

  initReplyPacket();

  if (value.canConvert< QVector<double> >()) {
    //    g_Application->printMessage(tr("replyFromVariant(QVector<double>)"));

    QVector<double> vec = value.value< QVector<double> > ();
    double v;

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_ARR_DOUBLE);
    m_Reply.rows = condSwapInt32(vec.size());
    m_Reply.cols = condSwapInt32(1);

    foreach (v, vec) {
      QByteArray a((const char*) &v, sizeof(double));

      if (m_SwapBytes) {
        for (int i=0; i<4; i++) {
          int tmp = a[i]; a[i] = a[7-i]; a[7-i] = tmp;
        }
      }

      m_ReplyData.append(a);
    }

    m_Reply.len = condSwapInt32(m_ReplyData.size());
  } else if (value.canConvert< QVector<QString> >()) {
    //     g_Application->print_message(tr("replyFromVariant(QVector<QString>)"));

    QVector<QString> vec = value.value< QVector<QString> > ();
    QString s;

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_ARR_STRING);
    m_Reply.rows = condSwapInt32(1);
    m_Reply.cols = condSwapInt32(vec.size());

    foreach (s, vec) {
      m_ReplyData.append(s);
      m_ReplyData.append('\0');
    }

    m_Reply.len = condSwapInt32(m_ReplyData.size());
  } else if (value.canConvert< QList<double> >()) {
//     g_Application->printMessage(tr("replyFromVariant(QList<double>)"));

    QList<double> vec = value.value< QList<double> > ();
    double v;

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_ARR_DOUBLE);
    m_Reply.rows = condSwapInt32(1);
    m_Reply.cols = condSwapInt32(vec.size());

    foreach (v, vec) {
      QByteArray a((const char*) &v, sizeof(double));

      if (m_SwapBytes) {
        for (int i=0; i<4; i++) {
          int tmp = a[i]; a[i] = a[7-i]; a[7-i] = tmp;
        }
      }

      m_ReplyData.append(a);
    }

    m_Reply.len = condSwapInt32(m_ReplyData.size());
  } else if (value.canConvert< QList<QString> >()) {
//     g_Application->printMessage(tr("replyFromVariant(QList<QString>)"));

    QList<QString> vec = value.value< QList<QString> > ();
    QString s;

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_ARR_STRING);
    m_Reply.rows = condSwapInt32(1);
    m_Reply.cols = condSwapInt32(vec.size());

    foreach (s, vec) {
      m_ReplyData.append(s);
      m_ReplyData.append('\0');
    }

    m_Reply.len = condSwapInt32(m_ReplyData.size());
  } else if (value.canConvert< QList<QVariant> >()) {
//     g_Application->printMessage(tr("replyFromVariant(QList<QString>)"));

    QList<QVariant> vec = value.value< QList<QVariant> > ();
    QVariant s;

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_ARR_DOUBLE);
    m_Reply.rows = condSwapInt32(1);
    m_Reply.cols = condSwapInt32(vec.size());

    foreach (s, vec) {
      double v=s.toDouble();
      QByteArray a((const char*) &v, sizeof(double));

      if (m_SwapBytes) {
        for (int i=0; i<4; i++) {
          int tmp = a[i]; a[i] = a[7-i]; a[7-i] = tmp;
        }
      }

      m_ReplyData.append(a);
    }

    m_Reply.len = m_ReplyData.size();
  } else {
//     g_Application->print_message(tr("replyFromVariant(QString)"));

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_STRING);

    strcpy((char*) &m_Reply.name, m_Packet.name);
    m_ReplyData.append(value.toString());
    m_ReplyData.append('\0');
    m_Reply.len = condSwapInt32(m_ReplyData.size());
  }

  sendReplyPacket();
}

void QSpecServer::replyFromError(QScriptValue value)
{
//  printf("Reply from error\n");

  initReplyPacket();

  m_Reply.cmd = condSwapInt32(SV_REPLY);
  m_Reply.type = condSwapInt32(SV_ERROR);

  strcpy((char*) &m_Reply.name, m_Packet.name);
//
//  QScriptValueIterator it(value);
//  while (it.hasNext()) {
//    it.next();
//    printf("Property %s = %s\n", qPrintable(it.name()), qPrintable(value.property(it.name()).toString()));
//  }
//  printf("Error message %s\n", qPrintable(value.property("message").toString()));

  m_ReplyData.append(value.property("message").toString());
  m_ReplyData.append('\0');
  m_Reply.len = condSwapInt32(m_ReplyData.size());
  m_Reply.err = 10;

  sendReplyPacket();
}

QVariant QSpecServer::readProperty(QString /*name*/)
{
  return QVariant();
}
