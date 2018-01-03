#include <QTcpSocket>
#include "qspecremote.h"
#include "spec_server.h"

QSpecRemote::QSpecRemote(QObject *parent, const char *name)
  : QObject(parent), 
    m_SerialNumber(0),
    m_ExpectHeader(true),
    m_SpecSocket(NULL),
    m_Packet(NULL)
{
  setObjectName(name);
  init();
}

void QSpecRemote::init()
{
  m_SpecSocket = new QTcpSocket(this);
  m_Packet     = new svr_head;

  connect(m_SpecSocket, &QAbstractSocket::hostFound, this, &QSpecRemote::hostFound);
  connect(m_SpecSocket, &QAbstractSocket::connected, this, &QSpecRemote::connected);
  connect(m_SpecSocket, &QAbstractSocket::disconnected, this, &QSpecRemote::disconnected);

  connectReadyRead();
//  connect(m_SpecSocket, &QIODevice::bytesWritten, this, &QSpecRemote::bytesWritten);
//  connect(m_SpecSocket, &QAbstractSocket::error,  this, &QSpecRemote::error);
}

void QSpecRemote::connectReadyRead()
{
  connect(m_SpecSocket, &QIODevice::readyRead, this, &QSpecRemote::readyRead);
}

void QSpecRemote::disconnectReadyRead()
{
  disconnect(m_SpecSocket, &QIODevice::readyRead, this, &QSpecRemote::readyRead);
}

int QSpecRemote::close()
{
  m_SpecSocket -> close();

  return 0;
}

int QSpecRemote::abort()
{
  return 0;
}

int QSpecRemote::connectToHost (const QString &host, const QString &portname)
{
  bool ok;
  int portnum;

  portnum = portname.toInt(&ok);

  if (ok) {
    m_SpecSocket -> disconnectFromHost();
    m_SpecSocket -> connectToHost(host, portnum);
  } else {
    emit socketStateChanged(-1);
  }

  if (sizeof(struct svr_head) != SV_CURR_SIZ+SV_NAME_LEN) {
    emit statusMessage(QString("svr_head data structure size(%1) != %2")
		       .arg(sizeof(struct svr_head)).arg(SV_CURR_SIZ+SV_NAME_LEN));
  }

  m_ExpectHeader = true;

  return 0;
}

QSpecRemote::~QSpecRemote()
{
  abort();
  close();

  delete m_SpecSocket;
  delete m_Packet;
}

int QSpecRemote::sendHello()
{
  struct svr_head s = { SV_SPEC_MAGIC, SV_VERSION, sizeof(svr_head),
                        ++m_SerialNumber, 0, 0,
			SV_HELLO, 0, 0, 0, 0,
			0, 0,
			"Hello from qspecfe" };

  quint64 nb = m_SpecSocket -> write((const char*) &s, sizeof(s));
  
  emit statusMessage(tr("bytes written = %1").arg(nb));

  if (nb != sizeof(s)) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }

  return m_SerialNumber;
}

void QSpecRemote::watchProperty(const QString& prop)
{
  struct svr_head s = { SV_SPEC_MAGIC, SV_VERSION, sizeof(svr_head),
                        ++m_SerialNumber, 0, 0,
			SV_REGISTER, 0, 0, 0, 0,
			0, 0,
			"" };
  strncpy(s.name, qPrintable(prop), sizeof(s.name));

  quint64 nb = m_SpecSocket -> write((const char*) &s, sizeof(s));
  
  if (nb != sizeof(s)) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }
}

QVariant QSpecRemote::getProperty(const QString &prop)
{
  disconnectReadyRead();

  struct svr_head s = { SV_SPEC_MAGIC, SV_VERSION, sizeof(svr_head),
                        ++m_SerialNumber, 0, 0,
			SV_CHAN_READ, 0, 0, 0, 0,
			0, 0,
			"" };
  strncpy(s.name, qPrintable(prop), sizeof(s.name));

  quint64 nb = m_SpecSocket -> write((const char*) &s, sizeof(s));
  
  if (nb != sizeof(s)) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }

  QVariant res = getReturnValue();

  connectReadyRead();

  return res;
}

void QSpecRemote::watchMotor(const QString &motor)
{
  watchProperty("motor/"+motor+"/position");
  watchProperty("motor/"+motor+"/dial_position");
  watchProperty("motor/"+motor+"/offset");
  watchProperty("motor/"+motor+"/step_size");
  watchProperty("motor/"+motor+"/sign");
  watchProperty("motor/"+motor+"/move_done");
  watchProperty("motor/"+motor+"/high_lim_hit");
  watchProperty("motor/"+motor+"/low_lim_hit");
  watchProperty("motor/"+motor+"/high_limit");
  watchProperty("motor/"+motor+"/low_limit");
}

void QSpecRemote::setProperty(const QString &prop, const QVariant &value)
{
  QString val = value.toString();

  quint64 extraLen=val.length()+1;

  struct svr_head s = { SV_SPEC_MAGIC, SV_VERSION, sizeof(svr_head),
                        ++m_SerialNumber, 0, 0,
                        SV_CHAN_SEND, SV_STRING, 0, 0, (unsigned) extraLen,
			0, 0,
			"" };
  strncpy(s.name, qPrintable(prop), sizeof(s.name));

  quint64 nb = m_SpecSocket -> write((const char*) &s, sizeof(s));
  
  if (nb != sizeof(s)) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }

  nb = m_SpecSocket -> write(qPrintable(val), extraLen);

  if (nb != extraLen) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }
}

void QSpecRemote::sendCmd(const QString &cmd)
{
  quint64 extraLen=cmd.length()+1;

  struct svr_head s = { SV_SPEC_MAGIC, SV_VERSION, sizeof(svr_head),
                        ++m_SerialNumber, 0, 0,
                        SV_CMD, SV_CMD, 0, 0, (unsigned) extraLen,
			0, 0,
			"" };

  quint64 nb = m_SpecSocket -> write((const char*) &s, sizeof(s));
  
  if (nb != sizeof(s)) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }

  nb = m_SpecSocket -> write(qPrintable(cmd), extraLen);

  if (nb != extraLen) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }
}

QVariant QSpecRemote::sendCmdReply(const QString &cmd)
{
  disconnectReadyRead();

  quint64 extraLen=cmd.length()+1;

  struct svr_head s = { SV_SPEC_MAGIC, SV_VERSION, sizeof(svr_head),
                        ++m_SerialNumber, 0, 0,
                        SV_CMD_WITH_RETURN, SV_STRING, 0, 0, (unsigned) extraLen,
			0, 0,
			"" };

  quint64 nb = m_SpecSocket -> write((const char*) &s, sizeof(s));
  
  if (nb != sizeof(s)) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }

  nb = m_SpecSocket -> write(qPrintable(cmd), extraLen);

  if (nb != extraLen) {
    emit statusMessage("socket write failed");
    emit socketStateChanged(-1);
  }

  QVariant res = getReturnValue();

  connectReadyRead();

  return res;
}

void QSpecRemote::hostFound()
{
  emit socketStateChanged(1);
}

void QSpecRemote::connected()
{
  emit socketStateChanged(2);
}

void QSpecRemote::disconnected()
{
  emit socketStateChanged(3);
}

QVariant QSpecRemote::getReturnValue()
{
  emit statusMessage(tr("QSpecRemote::getReturnValue(), %1 bytes available").arg(m_SpecSocket->bytesAvailable()));

  while (m_SpecSocket->waitForReadyRead()) {
    qint64 avail = m_SpecSocket->bytesAvailable();

    if (avail >= (int) sizeof(svr_head)) {
      quint64 nread = m_SpecSocket->read((char*) m_Packet, sizeof(svr_head));

      if (nread != sizeof(svr_head)) {
        emit statusMessage("read failure");
        emit socketStateChanged(-1);

        return QVariant();
      }

      QString msg;
      msg.sprintf("Packet SN %d : Cmd %d : Data Type %d : Data Length %d",
                  m_Packet->sn, m_Packet->cmd, m_Packet->type, m_Packet->len);
      emit statusMessage(msg);

      msg.sprintf("Packet sec %d : usec %d : rows %d : cols %d",
                  m_Packet->sec, m_Packet->usec, m_Packet->rows, m_Packet->cols);
      emit statusMessage(msg);

      msg.sprintf("Packet name %s", m_Packet->name);
      emit statusMessage(msg);

      if (m_Packet->magic != SV_SPEC_MAGIC) {
        emit statusMessage(QString("bad SPEC_MAGIC number 0x%1, should be 0x%2")
                           .arg(m_Packet->magic,8,16,QChar('0')).arg(SV_SPEC_MAGIC,8,16,QChar('0')));
        emit socketStateChanged(-1);
        return QVariant();
      }

      if (m_Packet->len > 0) {  // Expecting more?
        while (m_SpecSocket->bytesAvailable() < m_Packet->len) {
          m_SpecSocket->waitForReadyRead();
        }

        m_Data = m_SpecSocket->read(m_Packet->len);

        if (m_Packet->cmd != SV_REPLY) {
          dispatchPacket(); // Dispatch other packets...
        } else {
          if (m_Packet->sn != m_SerialNumber) {
            emit statusMessage(tr("Serial mismatch CMD:%1, REPLY:%2").arg(m_SerialNumber).arg(m_Packet->sn));
            emit socketStateChanged(-1);

            return QVariant();
          }

          switch(m_Packet->type) {
          case SV_DOUBLE:
            return *((double*) m_Data.data());
            break;

          case SV_STRING:
            return QString(m_Data);
            break;

          case SV_ERROR:
            msg.sprintf("Error: %s = %s", m_Packet->name, m_Data.data());
            emit statusMessage(msg);
            return QVariant();
            break;

          case SV_ASSOC:
            {
              char* p = m_Data.data();

              while (*p) {
                char* elem = p;
                p += strlen(p)+1;

                char* val = p;
                p += strlen(p)+1;

                msg.sprintf("%s[\"%s\"] = %s", m_Packet->name, elem, val);
                emit statusMessage(msg);
              }
            }
            break;

          case SV_ARR_DOUBLE:
            break;

          case SV_ARR_FLOAT:
            break;

          case SV_ARR_LONG:
            break;

          case SV_ARR_ULONG:
            break;

          case SV_ARR_SHORT:
            break;

          case SV_ARR_USHORT:
            break;

          case SV_ARR_CHAR:
            break;

          case SV_ARR_STRING:
            break;

          default:
            break;
          }

          return QVariant();
        }
      }
    }
  }

  return QVariant();
}

void QSpecRemote::dispatchPacket()
{
}

void QSpecRemote::readyRead()
{
  emit statusMessage(tr("QSpecRemote::readyRead(), %1 bytes available").arg(m_SpecSocket->bytesAvailable()));

  while (true) {
    emit statusMessage(tr("expecting header %1").arg(m_ExpectHeader));

    if (m_ExpectHeader) {
      emit statusMessage(tr("bytes available = %1").arg(m_SpecSocket->bytesAvailable()));
      emit statusMessage(tr("sizeof(svr_head) - %1").arg(sizeof(svr_head)));

      if (m_SpecSocket->bytesAvailable() >= (int) sizeof(svr_head)) {

        quint64 nread = m_SpecSocket->read((char *) m_Packet, sizeof(svr_head));

        emit statusMessage(tr("bytes read = %1").arg(nread));

	if (nread != sizeof(svr_head)) {
	  emit statusMessage("read failure");
	  emit socketStateChanged(-1);
	  return;
	}

 	QString msg;
 	msg.sprintf("Packet SN %d : Cmd %d : Data Type %d : Data Length %d", 
                    m_Packet->sn, m_Packet->cmd, m_Packet->type, m_Packet->len);
 	emit statusMessage(msg);

 	msg.sprintf("Packet sec %d : usec %d : rows %d : cols %d",
                    m_Packet->sec, m_Packet->usec, m_Packet->rows, m_Packet->cols);
 	emit statusMessage(msg);

        msg.sprintf("Packet name %s", m_Packet->name);
 	emit statusMessage(msg);

        if (m_Packet->magic != SV_SPEC_MAGIC) {
          emit statusMessage(QString("bad SPEC_MAGIC number 0x%1, should be 0x%2")
                             .arg(m_Packet->magic,8,16,QChar('0')).arg(SV_SPEC_MAGIC,8,16,QChar('0')));
	  emit socketStateChanged(-1);
	  return;
	}

        if (m_Packet->len > 0) {
          m_ExpectHeader = false;
	}
      } else {
	return;
      }
    } else {
      if (m_SpecSocket->bytesAvailable() >= m_Packet->len ) {
        m_Data.resize(m_Packet->len);
        quint64 len = m_SpecSocket->read(m_Data.data(), m_Packet->len);

        if (len != ((quint64) m_Packet->len)) {
	  emit statusMessage("data read failure");
	  emit socketStateChanged(-1);
	  return;
	} else {
//  	  if (data.size() >= 4) {
//  	    QString msg;
//  	    msg.sprintf("Data read: %02X %02X %02X %02X",
//  			data[0], data[1], data[2], data[3]);
//  	    emit statusMessage(msg);
//  	  }

	  QString msg;

          switch(m_Packet->type) {
	  case SV_DOUBLE:
            msg.sprintf("%s = %g", m_Packet->name, *((double*) m_Data.data()));
	    break;

	  case SV_STRING:
            msg.sprintf("%s = %s", m_Packet->name, m_Data.data());
	    emit statusMessage(msg);
	    break;

	  case SV_ERROR:
            msg.sprintf("Error: %s = %s", m_Packet->name, m_Data.data());
	    emit statusMessage(msg);
	    break;

	  case SV_ASSOC:
	    {
              char* p = m_Data.data();

	      while (*p) {
		char* elem = p;
		p += strlen(p)+1;

		char* val = p;
		p += strlen(p)+1;

                msg.sprintf("%s[\"%s\"] = %s", m_Packet->name, elem, val);
		emit statusMessage(msg);
	      }
	    }
	    break;

	  case SV_ARR_DOUBLE:
	    break;

	  case SV_ARR_FLOAT:
	    break;

	  case SV_ARR_LONG:
	    break;

	  case SV_ARR_ULONG:
	    break;

	  case SV_ARR_SHORT:
	    break;

	  case SV_ARR_USHORT:
	    break;

	  case SV_ARR_CHAR:
	    break;

	  case SV_ARR_STRING:
	    break;

	  default:
	    break;
	  }

          m_ExpectHeader = true;
	}
      } else {
	return;
      }
    }
  }

  emit socketStateChanged(5);
}

void QSpecRemote::bytesWritten( qint64  )
{
  emit socketStateChanged(6);
}

void QSpecRemote::error( QAbstractSocket::SocketError )
{
  emit socketStateChanged(-1);
}




