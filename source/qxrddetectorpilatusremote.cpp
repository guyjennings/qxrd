#include "qxrddetectorpilatusremote.h"

QxrdDetectorPilatusRemote::QxrdDetectorPilatusRemote(QcepObject *owner)
  : QcepObject("pilatusRemote", owner)
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

  m_Process.write(qPrintable(cmd));
}

void QxrdDetectorPilatusRemote::onReadyRead()
{
  printMessage("On ready read");

  while (m_Process.canReadLine()) {
    QString aLine = m_Process.readLine();

    printMessage(tr("Out: %1").arg(aLine));
  }
}
