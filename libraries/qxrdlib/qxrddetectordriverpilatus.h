#ifndef QXRDDETECTORDRIVERPILATUS_H
#define QXRDDETECTORDRIVERPILATUS_H

#include "qxrdlib_global.h"
#include "qxrddetectordriver.h"
#include <QTcpSocket>
#include "qxrddetectorpilatusremote-ptr.h"
#include "qxrdpilatussettings-ptr.h"

//TODO: move to plugin
class QXRD_EXPORT QxrdDetectorDriverPilatus : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdDetectorDriverPilatus(QString name,
                            QxrdPilatusSettingsWPtr pilatus,
                            QxrdExperimentWPtr expt,
                            QxrdAcquisitionWPtr acq);
  virtual ~QxrdDetectorDriverPilatus();

public slots:
  bool startDetectorDriver();
  bool stopDetectorDriver();

  bool changeExposureTime(double exposure);
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
  QxrdPilatusSettingsWPtr m_Pilatus;

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
