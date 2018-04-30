#ifndef QXRDPILATUSDRIVER_H
#define QXRDPILATUSDRIVER_H

#include "qxrddetectordriver.h"
#include "qxrdpilatussettings-ptr.h"
#include "qxrdpilatusremote-ptr.h"
#include <QTcpSocket>
#include "qxrdpilatusplugin-ptr.h"
#include "qcepimagedata-ptr.h"

class QxrdPilatusDriver : public QxrdDetectorDriver
{
  Q_OBJECT

public:
  QxrdPilatusDriver(QString name,
                    QxrdPilatusPluginWPtr plugin,
                    QxrdPilatusSettingsWPtr det,
                    QxrdExperimentWPtr expt,
                    QxrdAcqCommonWPtr acq);
  virtual ~QxrdPilatusDriver();

public slots:
  void haltDetectorDriver();
  void startDetectorDriver();
  void stopDetectorDriver();
  void changeExposureTime(double exposure);
  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();
  void beginFrame();

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
  QxrdPilatusPluginWPtr   m_Plugin;
  QxrdPilatusSettingsWPtr m_Pilatus;

  QTcpSocket m_PilatusSocket;
  QString    m_PilatusReply;
  QByteArray m_Buffer;
  QString    m_CurrentFile;

  double     m_ExposureTime;
  int        m_ExposuresPerFrame;
  int        m_ExposureFrameCount;

  QxrdPilatusRemotePtr m_Remote;

  QcepUInt32ImageDataPtr m_AccumulatedData;
};

Q_DECLARE_METATYPE(QxrdPilatusDriver*)

#endif // QXRDPILATUSDRIVER_H
