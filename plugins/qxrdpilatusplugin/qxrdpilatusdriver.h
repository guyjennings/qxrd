#ifndef QXRDPILATUSDRIVER_H
#define QXRDPILATUSDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdpilatussettings-ptr.h"
#include "qxrdpilatusremote-ptr.h"
#include <QTimer>
#include <QTcpSocket>

class QxrdPilatusDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdPilatusDriver(QString name,
                    QxrdDetectorSettingsWPtr det,
                    QxrdExperimentWPtr expt,
                    QxrdAcqCommonWPtr acq);
  virtual ~QxrdPilatusDriver();

public slots:
  virtual void startDetectorDriver();
  virtual void stopDetectorDriver();
  virtual void changeExposureTime(double exposure);
  virtual void beginAcquisition(double exposure);
  virtual void endAcquisition();
  virtual void shutdownAcquisition();
  virtual void beginFrame();

private:
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
  QxrdPilatusSettingsWPtr m_Pilatus;

  QTcpSocket m_PilatusSocket;
  QString    m_PilatusReply;
  QByteArray m_Buffer;
  QString    m_CurrentFile;

  double     m_ExposureTime;
  int        m_ExposuresPerFrame;
  int        m_ExposureFrameCount;

  QxrdPilatusRemotePtr m_Remote;
};

#endif // QXRDPILATUSDRIVER_H
