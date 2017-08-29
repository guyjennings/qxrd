#include "qxrddetectorpilatusremote.h"
#include "qxrddebug.h"
#include <QCryptographicHash>

QxrdDetectorPilatusRemote::QxrdDetectorPilatusRemote()
  : QcepObject("pilatusRemote"),
    m_FileTransferSize(0)
{
  printMessage("Constructed Pilatus Remote Object");

  connect(&m_Process, &QProcess::readyRead, this, &QxrdDetectorPilatusRemote::onReadyRead);
}

void QxrdDetectorPilatusRemote::connectToRemote(QString sshCmd)
{
  printMessage(tr("Connect to %1").arg(sshCmd));

  m_Process.setProcessChannelMode(QProcess::MergedChannels);
  m_Process.start(sshCmd);
}

void QxrdDetectorPilatusRemote::executeRemote(QString cmd)
{
  printMessage(tr("Exceute %1").arg(cmd));

  m_Process.write(qPrintable(cmd+"\n"));
}

void QxrdDetectorPilatusRemote::onReadyRead()
{
  printMessage(tr("Remote On ready read (%1 bytes available)").arg(m_Process.bytesAvailable()));

  m_Buffer.append(m_Process.readAll());

  while (1) {
    if (m_FileTransferSize > 0) { // Doing a file transfer
      if (m_Buffer.count() >= m_FileTransferSize) {
        m_TransferredFile = m_Buffer.left(m_FileTransferSize);

        QByteArray hash = QCryptographicHash::hash(m_TransferredFile, QCryptographicHash::Md5);

        printMessage(tr("%1 byte file transferred, md5sum %2")
                     .arg(m_FileTransferSize)
                     .arg(QString(hash.toHex())));

        m_Buffer.remove(0, m_FileTransferSize);

        m_FileTransferSize = 0;
      } else {
        return;
      }
    } else {
      int ind = m_Buffer.indexOf('\n');

      if (ind >= 0) {
        QString line = m_Buffer.left(ind); // Don't include newline
        m_Buffer.remove(0, ind+1);

        if (qcepDebug(DEBUG_PILATUS)) {
          printMessage(tr("(%1) : \"%2\"").arg(line.count()).arg(QString(line)));
        }

        interpretLine(line);
      } else { // No new lines when expected
        return;
      }
    }
  }
}

void QxrdDetectorPilatusRemote::interpretLine(QString line)
{
  printMessage(tr("QxrdDetectorPilatusRemote::interpretLine(\"%1\")").arg(line));

  if (line.startsWith("transfer:")) {
    QStringList fields = line.split(QRegExp("\\s+"));

    printMessage(tr("transfer: %1 args").arg(fields.count()));

    foreach(QString f, fields) {
      printMessage(tr("Arg:%1").arg(f));
    }

    m_FileTransferSize = fields.value(4).toInt();

    printMessage(tr("Transfer %1 bytes").arg(m_FileTransferSize));
  }
}
