#include <QTcpSocket>
#include <QTextStream>
#include "qspecserver.h"
#include <time.h>
#include <QVariant>

Q_DECLARE_METATYPE(QVector<double>);
Q_DECLARE_METATYPE(QVector<QString>);

QSpecServer::QSpecServer(QString name, QObject *parent)
  : QTcpServer(parent),
    m_ServerName(name),
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
	emit print_message(tr("Bound to port %1").arg(p));
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

  emit print_message(QString("New connection from %1")
		     .arg(m_Socket->peerAddress().toString()) );

  connect(m_Socket, SIGNAL(disconnected()),
	  this,     SLOT(connectionClosed()));
}

void
QSpecServer::connectionClosed()
{
  emit print_message("Client closed connection");
}

void
QSpecServer::clientRead()
{
  qint64 avail = m_Socket -> bytesAvailable();

//   emit print_message(tr("QSpecServer::clientRead, %1 bytes available").arg(avail));

  if (avail >= sizeof(struct svr_head)) {
    m_Socket -> read((char*) &m_Packet, sizeof(struct svr_head));
    m_Data.resize(0);

    avail -= sizeof(struct svr_head);

//     emit print_message(tr("QSpecServer::clientRead, %1 bytes available").arg(avail));

    readPacketData();
  }
}

int
QSpecServer::readPacketData()
{
  if (m_Packet.magic == SV_SPEC_MAGIC) {
    m_SwapBytes = 0;
  } else if (m_Packet.magic == swapInt32(SV_SPEC_MAGIC)) {
    m_SwapBytes = 1;
  } else {
    emit print_message(tr("Bad packet\n"));
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
//   emit print_message(tr("Packet received: magic %1, version %2, size %3")
// 		     .arg(m_Packet.magic).arg(m_Packet.vers).arg(m_Packet.size));
//   emit print_message(tr("Packet sn %1, sec %2, usec %3")
// 		     .arg(m_Packet.sn).arg(m_Packet.sec).arg(m_Packet.usec));
//   emit print_message(tr("Packet cmd %1, type %2, rows %3, cols %4, len %5")
// 		     .arg(m_Packet.cmd).arg(m_Packet.type).arg(m_Packet.rows).arg(m_Packet.cols).arg(m_Packet.len));

  switch (m_Packet.cmd) {
  case SV_CLOSE:
    emit print_message(tr("SV_CLOSE"));
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
    emit print_message(tr("SV_RETURN"));
    break;

  case SV_REGISTER:
    handle_register();
    break;

  case SV_UNREGISTER:
    handle_unregister();
    break;

  case SV_EVENT:
    emit print_message(tr("SV_EVENT"));
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
    emit print_message(tr("SV_REPLY"));
    break;

  case SV_HELLO:
    handle_hello();
    break;

  case SV_HELLO_REPLY:
    emit print_message(tr("SV_HELLO_REPLY"));
    break;

  default:
    emit print_message(tr("default"));
    break;
  }
}

void
QSpecServer::initReplyPacket()
{
  memset(&m_Reply, 0, sizeof(m_Reply));

  m_Reply.magic = SV_SPEC_MAGIC;
  m_Reply.vers  = 4;
  m_Reply.size  = sizeof(m_Reply);
  m_Reply.sn    = m_Packet.sn;
  
  struct timespec tm;

  clock_gettime(CLOCK_REALTIME, &tm);

  m_Reply.sec   = tm.tv_sec;
  m_Reply.usec  = tm.tv_nsec/1000;

  m_Reply.cmd   = 0;
  m_Reply.type  = SV_STRING;
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
  if (m_SwapBytes) {
    printf("Byte swapping not yet implemented for replies\n");
    return;
  }

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

void QSpecServer::handle_abort()
{
  emit print_message(tr("SV_ABORT"));
}

void QSpecServer::handle_cmd()
{
  emit print_message(tr("SV_CMD: %1").arg(QString(m_Data)));

  executeCommand(QString(m_Data));
}

void QSpecServer::handle_cmd_return()
{
  emit print_message(tr("SV_CMD_WITH_RETURN: %1").arg(QString(m_Data)));

  QVariant result = executeCommand(QString(m_Data));

  replyFromVariant(result);
}

void QSpecServer::handle_register()
{
  emit print_message(tr("SV_REGISTER: %1").arg(m_Packet.name));

  initReplyPacket();
  m_Reply.cmd = SV_EVENT;
  m_Reply.type= SV_STRING;

  strcpy((char*) &m_Reply.name, m_Packet.name);
  m_ReplyData.append("0");
  m_ReplyData.append('\0');
  m_Reply.len = m_ReplyData.size();

  sendReplyPacket();
}

void QSpecServer::handle_unregister()
{
  emit print_message(tr("SV_UNREGISTER: %1").arg(m_Packet.name));
}

void QSpecServer::handle_func()
{
  emit print_message(tr("SV_FUNC: %1").arg(m_Packet.name));
}

void QSpecServer::handle_func_return()
{
  emit print_message(tr("SV_FUNC_WITH_RETURN: %1").arg(m_Packet.name));

  initReplyPacket();
  m_Reply.cmd = SV_REPLY;
  m_Reply.type= SV_STRING;

  strcpy((char*) &m_Reply.name, m_Packet.name);
  m_ReplyData.append("0");
  m_ReplyData.append('\0');
  m_Reply.len = m_ReplyData.size();

  sendReplyPacket();  
}

void QSpecServer::handle_read()
{
  emit print_message(tr("SV_CHAN_READ: %1").arg(m_Packet.name));

  QVariant res = readProperty( m_Packet.name);

  replyFromVariant(res);
}

void QSpecServer::handle_send()
{
  emit print_message(tr("SV_CHAN_SEND: %1").arg(m_Packet.name));
}

void QSpecServer::handle_hello()
{
  emit print_message(tr("SV_HELLO: %1").arg(m_Packet.name));

  initReplyPacket();

  m_Reply.cmd = SV_HELLO_REPLY;
  m_Reply.type= SV_STRING;

  strcpy((char*) &m_Reply.name, qPrintable(m_ServerName));
  m_ReplyData.append(m_ServerName);
  m_ReplyData.append('\0');
  m_Reply.len = m_ReplyData.size();

  sendReplyPacket();
}

void QSpecServer::replyFromVariant(QVariant value)
{
//   emit print_message(tr("replyFromVariant(%1)").arg(value.typeName()));

  initReplyPacket();

  if (value.canConvert< QVector<double> >()) {
    emit print_message(tr("replyFromVariant(QVector<double>)"));

    QVector<double> vec = value.value< QVector<double> > ();
    double v;

    m_Reply.cmd = SV_REPLY;
    m_Reply.type= SV_ARR_DOUBLE;
    m_Reply.rows = vec.size();
    m_Reply.cols = 1;

    foreach (v, vec) {
      QByteArray a((const char*) &v, sizeof(double));
      m_ReplyData.append(a);
    }

    m_Reply.len = m_ReplyData.size();
  } else if (value.canConvert< QVector<QString> >()) {
//     emit print_message(tr("replyFromVariant(QVector<QString>)"));

    QVector<QString> vec = value.value< QVector<QString> > ();
    QString s;

    m_Reply.cmd = SV_REPLY;
    m_Reply.type= SV_ARR_STRING;
    m_Reply.rows = 1;
    m_Reply.cols = vec.size();

    foreach (s, vec) {
      m_ReplyData.append(s);
      m_ReplyData.append('\0');
    }

    m_Reply.len = m_ReplyData.size();
  } else {
//     emit print_message(tr("replyFromVariant(QString)"));

    m_Reply.cmd = SV_REPLY;
    m_Reply.type= SV_STRING;

    strcpy((char*) &m_Reply.name, m_Packet.name);
    m_ReplyData.append(value.toString());
    m_ReplyData.append('\0');
    m_Reply.len = m_ReplyData.size();
  }

  sendReplyPacket();
}

QVariant QSpecServer::executeCommand(QString cmd)
{
  return 17;
}

QVariant QSpecServer::readProperty(QString name)
{
  return QVariant();
}
