#ifndef QXRDDETECTORDRIVERPILATUS_H
#define QXRDDETECTORDRIVERPILATUS_H

#include "qxrddetectordriver.h"
#include <QTcpSocket>
#include "qxrddetectorpilatusremote-ptr.h"
#include "qxrddetectorsettingspilatus-ptr.h"

class QxrdDetectorDriverPilatus : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverPilatus(QString name,
                            QxrdDetectorSettingsPilatusPtr pilatus,
                            QxrdExperimentPtr expt,
                            QxrdAcquisitionPtr acq);

public slots:
  bool startDetectorDriver();
  bool stopDetectorDriver();

  bool beginAcquisition(double exposure);
  bool endAcquisition();
  bool shutdownAcquisition();

private:
  void beginFrame();

  void remoteConnect  (QString sshCmd);
  void remoteCommand  (QString cmd);
  void remoteCopy     (QString file);
  void remoteDelete   (QString file);
  void remoteTransfer (QString file);

  void executeCommand (QString cmd);

private:
  enum {
    NoExternalTrigger,
    ExternalTrigger,
    ExternalEnable
  };

private:
  void beginExposure(double exposure);
  void interpretReply(QString reply);
  void expose();

  void imagePath(QString path);

  void sendCommand(QString cmd);

  void readyRead();

  void loadAndPush(QString f);

private:
  QxrdDetectorSettingsPilatusWPtr m_Pilatus;

  QTcpSocket m_PilatusSocket;
  QString    m_PilatusReply;
  QByteArray m_Buffer;
  QString    m_CurrentFile;

  double     m_ExposureTime;
  int        m_ExposuresPerFrame;
  int        m_ExposureFrameCount;

  QxrdDetectorPilatusRemotePtr m_Remote;
};

#endif // QXRDDETECTORDRIVERPILATUS_H
