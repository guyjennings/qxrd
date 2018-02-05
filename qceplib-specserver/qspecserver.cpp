#define _CRT_SECURE_NO_WARNINGS

#include "qcepdebug.h"
#include "qspecserver.h"
//#include "qxrdserverthread.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QVariant>
#include <QDateTime>
#include <QTime>
#include <QNetworkInterface>
#include <QHostInfo>
#include "qcepexperiment.h"
#include "qcepobject.h"

QSpecServer::QSpecServer(QString name)
  : QTcpServer(NULL),
    m_Owner(),
    m_ServerName(name),
    m_Socket(NULL),
    m_Address(QHostAddress::Any),
    m_Port(-1),
    m_SwapBytes(0),
    m_ReplyHeadSent(0)
{
  init_svr_head(&m_Packet);
  init_svr_head(&m_Reply);

  connect(this, &QTcpServer::newConnection, this, &QSpecServer::openNewConnection);
}

void QSpecServer::initialize(QObjectWPtr owner)
{
  m_Owner =
      qSharedPointerDynamicCast<QcepObject>(owner);
}

QSpecServer::~QSpecServer()
{
//  stopServer();
}

void
QSpecServer::startServer(QHostAddress a, int pmin, int pmax)
{
  setMaxPendingConnections(1);

  if (isListening()) {
    close();
  }

  if (pmin < 0) {
    pmin = 6510;

    if (pmax < 0) {
      pmax = 6530;
    }
  } else {
    if (pmax < 0) {
      pmax = pmin;
    }
  }

  for (int p=pmin; p<=pmax; p++) {
    if (listen(a, p)) {
      printMessage(tr("Started SPEC Server on address %1 port %2")
                   .arg(a.toString()).arg(p));

      m_Address = a;
      m_Port = p;

      reportServerAddress();

      break;
    } else {
      m_Port = -1;
    }
  }

  if (m_Port == -1) {
    if (pmin == pmax) {
      printMessage(tr("Failed to bind to address %1 port %2\nIs there another copy running already?")
                   .arg(a.toString()).arg(pmin));
    } else {
      printMessage(tr("Could not find unused port on address %1 between %2 and %3\nAre there other copies running already?")
                   .arg(a.toString()).arg(pmin).arg(pmax));
    }
  }
}

void
QSpecServer::stopServer()
{
  if (isListening()) {
    close();
  }
}

int
QSpecServer::port()
{
  return m_Port;
}

void
QSpecServer::reportServerAddress()
{
  if (m_Port >= 0) {
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    foreach (QNetworkInterface interface, interfaces) {
      if (interface.flags().testFlag(QNetworkInterface::IsRunning) &&
          interface.flags().testFlag(QNetworkInterface::IsUp) &&
          !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

        QList<QNetworkAddressEntry> addresses = interface.addressEntries();

        foreach (QNetworkAddressEntry address, addresses) {
          QString addr = address.ip().toString();

          QHostInfo hinfo = QHostInfo::fromName(addr);

          addr = hinfo.hostName();

          if (address.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            printMessage(tr("Access at \"%1:%2\" or \"%3:%4\"")
                              .arg(addr).arg(m_Port)
                              .arg(addr).arg(m_ServerName));
          }
        }
      }
    }
  }
}

void
QSpecServer::printMessage(QString msg, QDateTime ts)
{
  QcepObjectPtr owner(m_Owner);

  if (owner) {
    owner->printMessage(msg, ts);
  }
}

/**
  Called when a new connection is opened from SPEC

  Call TCP_NODELAY to turn off Nagle algorithm on the
  open socket to improve the latency of the command stream
  */

void
QSpecServer::openNewConnection()
{
  m_Socket = nextPendingConnection();

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("LowDelayOption = %1").arg(m_Socket->socketOption(QAbstractSocket::LowDelayOption).toString()));
  }

  m_Socket->setSocketOption(QAbstractSocket::LowDelayOption, (int) 1);

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("LowDelayOption = %1").arg(m_Socket->socketOption(QAbstractSocket::LowDelayOption).toString()));
  }

  connect(m_Socket, &QIODevice::readyRead, this, &QSpecServer::clientRead);

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(QString("New connection from %1")
                                .arg(m_Socket->peerAddress().toString()) );
  }
}

void
QSpecServer::connectionClosed()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage("Client closed connection");
  }
}

void
QSpecServer::clientRead()
{
  if (m_ReplyHeadSent != 0) {
    printMessage(tr("QSpecServer::clientRead, m_ReplyHeadSent != 0"));
  }

  quint64 avail = m_Socket -> bytesAvailable();

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::clientRead, %1 bytes available").arg(avail));
  }

  while (avail >= sizeof(struct svr_head)) {
    m_Socket -> read((char*) &m_Packet, sizeof(struct svr_head));
    m_Data.resize(0);

    avail = m_Socket -> bytesAvailable();

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("QSpecServer::clientRead, read header, %1 bytes available").arg(avail));
    }

    readPacketData();

    avail = m_Socket -> bytesAvailable();
  }
}

int
QSpecServer::readPacketData()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::readPacketData start"));
  }

  if (m_Packet.magic == SV_SPEC_MAGIC) {
    m_SwapBytes = 0;
  } else if (m_Packet.magic == swapUInt32(SV_SPEC_MAGIC)) {
    m_SwapBytes = 1;
  } else {
    printMessage(tr("Bad packet\n"));
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

  initReplyPacket();

  if (m_Packet.len > 0) {
    sendReplyPacketHead();

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("QSpecServer::readPacketData m_Packet.len = %1").arg(m_Packet.len));
    }

    while (m_Packet.len > m_Socket->bytesAvailable()) {
      if (qcepDebug(DEBUG_SERVER)) {
        printMessage(tr("QSpecServer::readPacketData available = %1").arg(m_Socket->bytesAvailable()));
      }
      m_Socket -> waitForReadyRead();
    }

    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("QSpecServer::readPacketData about to read %1 bytes").arg(m_Packet.len));
    }

    m_Data.append(m_Socket->read(m_Packet.len));
  }

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::readPacketData end"));
  }

  return interpretPacket();
}

/**
  Handle the various kinds of input packets
  */

int
QSpecServer::interpretPacket()
{

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::interpretPacket start"));
  }

  switch (m_Packet.cmd) {
  case SV_CLOSE:
//    printMessage(tr("SV_CLOSE"));
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
//    printMessage(tr("SV_RETURN"));
    break;

  case SV_REGISTER:
    handle_register();
    break;

  case SV_UNREGISTER:
    handle_unregister();
    break;

  case SV_EVENT:
//    printMessage(tr("SV_EVENT"));
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
//    printMessage(tr("SV_REPLY"));
    break;

  case SV_HELLO:
    handle_hello();
    break;

  case SV_HELLO_REPLY:
//    printMessage(tr("SV_HELLO_REPLY"));
    break;

  default:
//    printMessage(tr("default"));
    break;
  }

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::interpretPacket end"));
  }

  return 0;
}

/**
  Initialize the reply packet based on the byte order
  of the received packet
  */

void
QSpecServer::initReplyPacket()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::initReplyPacket"));
  }

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

  m_ReplyHeadSent = 0;
}

/**
  Send back the first few bytes of the reply packet immediately,
  so that the initial packet will get an ACK.  This avoids
  an 0.2 sec delay that would occur due to delayed ACK on
  Windows
  */

#define HEAD_SIZE sizeof(m_Reply.magic)

void
QSpecServer::sendReplyPacketHead()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::sendReplyPacketHead start"));
  }

  m_Socket -> write((char*) &m_Reply, HEAD_SIZE);
  m_Socket -> flush();
  m_ReplyHeadSent = HEAD_SIZE;

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::sendReplyPacketHead end"));
  }
}

/**
  Send back the remainder of the reply packet, omitting any bytes
  that have already been sent by sendReplyPacketHead
  */

void
QSpecServer::sendReplyPacketTail()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::sendReplyPacketTail start"));
  }

  m_Socket -> write(((char*) &m_Reply)+m_ReplyHeadSent, sizeof(m_Reply) - m_ReplyHeadSent);
  m_Socket -> write(m_ReplyData);
  m_Socket -> flush();

  m_ReplyHeadSent = 0;

  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("QSpecServer::sendReplyPacketTail end"));
  }
}

/**
  Swap byte order
  */

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

/**
  Conditionally swap byte order - depending
  on the byte order of the received packet
  */

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
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_ABORT"));
  }
}

void QSpecServer::handle_cmd()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_CMD: %1").arg(QString(m_Data)));
  }

  emit executeCommand(QString(m_Data));
}

void QSpecServer::handle_cmd_return()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_CMD_WITH_RETURN: %1").arg(QString(m_Data)));
  }

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
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_REGISTER: %1").arg(m_Packet.name));
  }

  m_Reply.cmd = condSwapInt32(SV_EVENT);
  m_Reply.type= condSwapInt32(SV_STRING);

  strcpy((char*) &m_Reply.name, m_Packet.name);
  m_ReplyData.append("0");
  m_ReplyData.append('\0');
  m_Reply.len = condSwapInt32(m_ReplyData.size());

  sendReplyPacketTail();
}

void QSpecServer::handle_unregister()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_UNREGISTER: %1").arg(m_Packet.name));
  }
}

void QSpecServer::handle_func()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_FUNC: %1").arg(m_Packet.name));
  }
}

void QSpecServer::handle_func_return()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_FUNC_WITH_RETURN: %1").arg(m_Packet.name));
  }

  m_Reply.cmd = condSwapInt32(SV_REPLY);
  m_Reply.type= condSwapInt32(SV_STRING);

  strcpy((char*) &m_Reply.name, m_Packet.name);
  m_ReplyData.append("0");
  m_ReplyData.append('\0');
  m_Reply.len = condSwapInt32(m_ReplyData.size());

  sendReplyPacketTail();
}

void QSpecServer::handle_read()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_CHAN_READ: %1").arg(m_Packet.name));
  }

  QVariant res = readProperty( m_Packet.name);

  replyFromVariant(res);
}

void QSpecServer::handle_send()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_CHAN_SEND: %1").arg(m_Packet.name));
  }
}

void QSpecServer::handle_hello()
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("SV_HELLO: %1").arg(m_Packet.name));
  }

  m_Reply.cmd = condSwapInt32(SV_HELLO_REPLY);
  m_Reply.type= condSwapInt32(SV_STRING);

  strncpy((char*) &m_Reply.name, qPrintable(m_ServerName), sizeof(m_Reply.name)-1);

  m_ReplyData.append(m_ServerName);
  m_ReplyData.append('\0');
  m_Reply.len = condSwapInt32(m_ReplyData.size());

  sendReplyPacketTail();
}

void QSpecServer::replyFromVariant(QVariant value)
{
  if (qcepDebug(DEBUG_SERVER)) {
    printMessage(tr("replyFromVariant(%1)").arg(value.typeName()));
  }

  /*if (value.isNull()) {
  } else*/ if (value.canConvert< QVector<double> >()) {
    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("replyFromVariant(QVector<double>)"));
    }

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
    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("replyFromVariant(QVector<QString>)"));
    }

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
    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("replyFromVariant(QList<double>)"));
    }

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
    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("replyFromVariant(QList<QString>)"));
    }

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
    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("replyFromVariant(QList<QVariant>)"));
    }

    QList<QVariant> vec = value.value< QList<QVariant> > ();
    QVariant s;

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_ASSOC);
    m_Reply.rows = condSwapInt32(1);
    m_Reply.cols = condSwapInt32(1);

//    foreach (s, vec) {
//      double v=s.toDouble();
//      QByteArray a((const char*) &v, sizeof(double));

//      if (m_SwapBytes) {
//        for (int i=0; i<4; i++) {
//          int tmp = a[i]; a[i] = a[7-i]; a[7-i] = tmp;
//        }
//      }

//      m_ReplyData.append(a);
//    }

    for (int i=0; i<vec.length(); i++) {
      QString val = vec[i].toString();
      m_ReplyData.append(tr("%1").arg(i));
      m_ReplyData.append('\0');
      m_ReplyData.append(val);
      m_ReplyData.append('\0');
    }
    m_ReplyData.append('\0');

    m_Reply.len = condSwapInt32(m_ReplyData.size());
  } else {
    if (qcepDebug(DEBUG_SERVER)) {
      printMessage(tr("replyFromVariant(QString)"));
    }

    m_Reply.cmd = condSwapInt32(SV_REPLY);
    m_Reply.type= condSwapInt32(SV_STRING);

    strcpy((char*) &m_Reply.name, m_Packet.name);
    m_ReplyData.append(value.toString());
    m_ReplyData.append('\0');
    m_Reply.len = condSwapInt32(m_ReplyData.size());
  }

  sendReplyPacketTail();
}

void QSpecServer::replyFromError(QScriptValue value)
{
//  printf("Reply from error\n");

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

  sendReplyPacketTail();
}

QVariant QSpecServer::readProperty(QString /*name*/)
{
  return QVariant();
}

void QSpecServer::init_svr_head(svr_head *h)
{
  if (h) {
    h->magic = SV_SPEC_MAGIC;
    h->vers  = 4;
    h->size  = 0;
    h->sn    = 0;
    h->sec   = 0;
    h->usec  = 0;

    h->cmd   = 0;
    h->type  = SV_ERROR;
    h->rows  = 0;
    h->cols  = 0;
    h->len   = 0;
    h->err   = 0;
    h->flags = 0;
    strncpy(h->name, "", 2);
  }
}
