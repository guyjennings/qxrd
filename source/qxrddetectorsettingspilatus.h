#ifndef QXRDDETECTORSETTINGSPILATUS_H
#define QXRDDETECTORSETTINGSPILATUS_H

#include "qcepmacros.h"
#include "qxrddetectorsettings.h"
#include <QTcpSocket>
#include "qxrddetectorpilatusremote-ptr.h"

class QxrdDetectorSettingsPilatus : public QxrdDetectorSettings
{
  Q_OBJECT
public:
  explicit QxrdDetectorSettingsPilatus(QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum);
  virtual ~QxrdDetectorSettingsPilatus();

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);

  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

signals:

public slots:
  void startDetector();
  void stopDetector();

  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();

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

  void beginExposure(double exposure);
  void interpretReply(QString reply);
  void expose();

  void imagePath(QString path);

  void sendCommand(QString cmd);

  void readyRead();

private:
  void loadAndPush(QString f);

private:
  QTcpSocket m_PilatusSocket;
  QString    m_PilatusReply;
  QByteArray m_Buffer;
  QString    m_CurrentFile;

  double     m_ExposureTime;
  int        m_ExposuresPerFrame;
  int        m_ExposureFrameCount;

  QxrdDetectorPilatusRemotePtr m_Remote;

public:
  Q_PROPERTY(QString pilatusHost READ get_PilatusHost WRITE set_PilatusHost)
  QCEP_STRING_PROPERTY(PilatusHost)

  Q_PROPERTY(int pilatusPort READ get_PilatusPort WRITE set_PilatusPort)
  QCEP_INTEGER_PROPERTY(PilatusPort)

  Q_PROPERTY(QString pilatusUser READ get_PilatusUser WRITE set_PilatusUser)
  QCEP_STRING_PROPERTY(PilatusUser)

  Q_PROPERTY(QString pilatusSSH READ get_PilatusSSH WRITE set_PilatusSSH)
  QCEP_STRING_PROPERTY(PilatusSSH)

  Q_PROPERTY(QString pilatusSCP READ get_PilatusSCP WRITE set_PilatusSCP)
  QCEP_STRING_PROPERTY(PilatusSCP)

  Q_PROPERTY(QString pilatusDataDirectory READ get_PilatusDataDirectory WRITE set_PilatusDataDirectory)
  QCEP_STRING_PROPERTY(PilatusDataDirectory)

  Q_PROPERTY(bool readFilesLocally READ get_ReadFilesLocally WRITE set_ReadFilesLocally)
  QCEP_BOOLEAN_PROPERTY(ReadFilesLocally)

  Q_PROPERTY(bool deleteFilesAfterReading READ get_DeleteFilesAfterReading WRITE set_DeleteFilesAfterReading)
  QCEP_BOOLEAN_PROPERTY(DeleteFilesAfterReading)

  Q_PROPERTY(int exposureMode READ get_ExposureMode WRITE set_ExposureMode)
  QCEP_INTEGER_PROPERTY(ExposureMode)

  Q_PROPERTY(double enableFrequency READ get_EnableFrequency WRITE set_EnableFrequency)
  QCEP_DOUBLE_PROPERTY(EnableFrequency)

  Q_PROPERTY(QString pilatusExtension READ get_PilatusExtension WRITE set_PilatusExtension)
  QCEP_STRING_PROPERTY(PilatusExtension)
};

#endif // QXRDDETECTORPILATUS_H