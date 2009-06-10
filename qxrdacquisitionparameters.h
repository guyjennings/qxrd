#ifndef QXRDACQUISITIONPARAMETERS_H
#define QXRDACQUISITIONPARAMETERS_H

#include <QObject>
#include <QMutex>

class QxrdSettings;

class QxrdAcquisitionParameters : public QObject
{
  Q_OBJECT;
public:
  QxrdAcquisitionParameters();
  typedef QObject inherited;

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

  void fileIndexChanged(int index);
  void exposureTimeChanged(double t);
  void readoutModeChanged(int mode);
  void summedExposuresChanged(int nsummed);
  void filesInSequenceChanged(int nframes);
  void filePatternChanged(QString pattern);
  void outputDirectoryChanged(QString path);
  void darkSummedExposuresChanged(int nsummed);
  void cameraGainChanged(int gain);

public slots:
  void    setFileIndex(int index);
  void    changeFileIndex(int index);
  void    setFilesInSequence(int nframes);
  void    changeFilesInSequence(int nframes);
  void    setSummedExposures(int nsummed);
  void    changeSummedExposures(int nsummed);
  void    setReadoutMode(int mode);
  void    changeReadoutMode(int mode);
  void    setExposureTime(double t);
  void    changeExposureTime(double t);
  void    setFilePattern(QString pattern);
  void    changeFilePattern(QString pattern);
  void    setOutputDirectory(QString path);
  void    changeOutputDirectory(QString path);
  void    setDarkSummedExposures(int nsummed);
  void    changeDarkSummedExposures(int nsummed);
  void    setCameraGain(int mode);
  void    changeCameraGain(int mode);

public:
  void readSettings(QxrdSettings *settings, QString section);
  void saveSettings(QxrdSettings *settings, QString section);

  double  exposureTime() const;
  int     readoutMode() const;
  int     summedExposures() const;
  int     filesInSequence() const;
  int     fileIndex() const;
  QString filePattern() const;
  QString outputDirectory() const;
  int     darkSummedExposures() const;
  int     cameraGain() const;

  void    setDebug(int dbg);
  int     debug() const;


//  void    setCameraMode(int mode);
//  int     cameraMode() const;
//
//  void    setFrameSyncMode(int mode);
//  int     frameSyncMode() const;
//
//  void    setTimerSync(int mode);
//  int     timerSync() const;

  Q_PROPERTY(int    debug            READ debug WRITE setDebug);
  Q_PROPERTY(double exposureTime     READ exposureTime WRITE setExposureTime);
  Q_PROPERTY(int    readoutMode      READ readoutMode WRITE setReadoutMode);
  Q_PROPERTY(int    summedExposures  READ summedExposures WRITE setSummedExposures);
  Q_PROPERTY(int    filesInSequence  READ filesInSequence WRITE setFilesInSequence);
  Q_PROPERTY(int    fileIndex        READ fileIndex WRITE setFileIndex);
  Q_PROPERTY(QString filePattern     READ filePattern WRITE setFilePattern);
  Q_PROPERTY(QString outputDirectory READ outputDirectory WRITE setOutputDirectory);
  Q_PROPERTY(int     darkSummedExposures READ darkSummedExposures WRITE setDarkSummedExposures);
  Q_PROPERTY(int     cameraGain      READ cameraGain WRITE setCameraGain);

//  Q_PROPERTY(int     cameraMode      READ cameraMode WRITE setCameraMode);
//  Q_PROPERTY(int     frameSyncMode   READ frameSyncMode WRITE setFrameSyncMode);
//  Q_PROPERTY(int     timerSync       READ timerSync WRITE setTimerSync);

private:
  mutable QMutex         m_Mutex;
  int                    m_Debug;
  QString                m_OutputDirectory;
  QString                m_FilePattern;
  int                    m_FileIndex;
//  int                    m_CameraMode;
//  int                    m_FrameSyncMode;
//  int                    m_TimerSync;
  int                    m_CameraGain;
  double                 m_ExposureTime;
  int                    m_ReadoutMode;
  int                    m_SummedExposures;
  int                    m_DarkSummedExposures;
  int                    m_FilesInSequence;
};

#endif // QXRDACQUISITIONPARAMETERS_H
