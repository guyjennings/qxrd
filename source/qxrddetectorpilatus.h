#ifndef QXRDDETECTORPILATUS_H
#define QXRDDETECTORPILATUS_H

#include "qcepmacros.h"
#include "qxrddetector.h"
#include <QTcpSocket>
//#include <QFileSystemWatcher>
#include <QTimer>

class QxrdDetectorPilatus : public QxrdDetector
{
  Q_OBJECT
public:
  explicit QxrdDetectorPilatus(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt, QxrdAcquisitionWPtr acq, int detNum, QcepObject *parent);
  virtual ~QxrdDetectorPilatus();

  void pushPropertiesToProxy(QxrdDetectorProxyPtr proxy);
  void pullPropertiesfromProxy(QxrdDetectorProxyPtr proxy);
  static void pushDefaultsToProxy(QxrdDetectorProxyPtr proxy);

signals:

public slots:
  void onExposureTimeChanged();

  void startDetector();
  void stopDetector();

//  void acquire();
  void beginAcquisition(double exposure);
  void endAcquisition();
  void shutdownAcquisition();

  void beginFrame();

private:
  enum {
    NoExternalTrigger,
    ExternalTrigger,
    ExternalEnable
  };

  void beginExposure(double exposure);
  void interpretReply(QString reply);
  void expose();

  void exposureTime(double exposure);
  void exposurePeriod(double period);
  void exposureDelay(double delay);
  void exposuresPerFrame(int nexp);
  void exposureFrameCount(int nfram);

  void imagePath(QString path);
  void exposure(QString file);
  void extTrigger(QString file);
  void extEnable(QString file);

  void acquireImage(QString fileName, double exposure);

  void    sendCommand(QString cmd);
  QString sendCommandReply(QString cmd);

  QString reply();
  void expectReply(QString regexp);

  void readyRead();
//  void fileChanged(const QString &path);
//  void directoryChanged(const QString &path);
  void checkExpectedFiles();

private:
  void pushFileExpected(QString f);

private:
  QTcpSocket m_PilatusSocket;
  QString    m_PilatusReply;
//  QFileSystemWatcher m_FileWatcher;
  QByteArray m_Preread;
  QString    m_CurrentFile;
  QStringList m_ExpectedFiles;
  QTimer     m_ExpectedFileTimer;

  double     m_ExposureTime;
  int        m_ExposuresPerFrame;
  int        m_ExposureFrameCount;

public:
  Q_PROPERTY(QString pilatusHost READ get_PilatusHost WRITE set_PilatusHost)
  QCEP_STRING_PROPERTY(PilatusHost)

  Q_PROPERTY(int pilatusPort READ get_PilatusPort WRITE set_PilatusPort)
  QCEP_INTEGER_PROPERTY(PilatusPort)

  Q_PROPERTY(QString pilatusFilePattern READ get_PilatusFilePattern WRITE set_PilatusFilePattern)
  QCEP_STRING_PROPERTY(PilatusFilePattern)

  Q_PROPERTY(QString pilatusDataDirectory READ get_PilatusDataDirectory WRITE set_PilatusDataDirectory)
  QCEP_STRING_PROPERTY(PilatusDataDirectory)

  Q_PROPERTY(bool readFilesLocally READ get_ReadFilesLocally WRITE set_ReadFilesLocally)
  QCEP_BOOLEAN_PROPERTY(ReadFilesLocally)

  Q_PROPERTY(QString localDataDirectory READ get_LocalDataDirectory WRITE set_LocalDataDirectory)
  QCEP_STRING_PROPERTY(LocalDataDirectory)

  Q_PROPERTY(bool deleteFilesAfterReading READ get_DeleteFilesAfterReading WRITE set_DeleteFilesAfterReading)
  QCEP_BOOLEAN_PROPERTY(DeleteFilesAfterReading)

  Q_PROPERTY(int exposureMode READ get_ExposureMode WRITE set_ExposureMode)
  QCEP_INTEGER_PROPERTY(ExposureMode)

  Q_PROPERTY(double enableFrequency READ get_EnableFrequency WRITE set_EnableFrequency)
  QCEP_DOUBLE_PROPERTY(EnableFrequency)
};

#endif // QXRDDETECTORPILATUS_H
