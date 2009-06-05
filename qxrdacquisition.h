#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QVector>
#include <QFuture>
#include <QScriptEngine>

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"

class QxrdApplication;
class QxrdAcquisitionThread;
class QxrdWindow;

class QxrdAcquisition : public QObject
{
  Q_OBJECT;

 public:
  QxrdAcquisition(QxrdAcquisitionThread *thread);
  ~QxrdAcquisition();

 public slots:
  void initialize();
  void acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);
  void cancel();
  void cancelDark();
  void evaluate(QString cmd);

 signals:
  void acquireStarted(int dark);
  void acquireComplete(int dark);
  void printMessage(QString msg);
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void statusMessage(QString msg);

  void fileIndexChanged(int index);
  void exposureTimeChanged(double t);
  void integrationModeChanged(int mode);
  void nSummedChanged(int nsummed);
  void nFramesChanged(int nframes);
  void filePatternChanged(QString pattern);
  void outputDirectoryChanged(QString path);
  void darkNSummedChanged(int nsummed);

public slots:
  void    setFileIndex(int index);
  void    setNFrames(int nframes);
  void    setNSummed(int nsummed);
  void    setIntegrationMode(int mode);
  void    setExposureTime(double t);
  void    setFilePattern(QString pattern);
  void    setOutputDirectory(QString path);
  void    setDarkNSummed(int nsummed);

public:
  bool onEndFrame();
  void onEndAcquisition();
  QVector<double> integrationTimes();
  int acquisitionStatus(double time);
  bool canStart();
  void onEndFrameCallback();

  void    setCameraMode(int mode);
  int     cameraMode() const;

  void    setFrameSyncMode(int mode);
  int     frameSyncMode() const;

  void    setTimerSync(int mode);
  int     timerSync() const;

  void    setCameraGain(int mode);
  int     cameraGain() const;

  double  exposureTime() const;
  int     integrationMode() const;
  int     nSummed() const;
  int     nFrames() const;
  int     fileIndex() const;
  QString filePattern() const;
  QString outputDirectory() const;
  int     darkNSummed() const;

  void    setDebug(int dbg);
  int     debug() const;

  Q_PROPERTY(int    debug            READ debug WRITE setDebug);
  Q_PROPERTY(double exposureTime     READ exposureTime WRITE setExposureTime);
  Q_PROPERTY(int    integrationMode  READ integrationMode WRITE setIntegrationMode);
  Q_PROPERTY(int    nSummed          READ nSummed WRITE setNSummed);
  Q_PROPERTY(int    nFrames          READ nFrames WRITE setNFrames);
  Q_PROPERTY(int    fileIndex        READ fileIndex WRITE setFileIndex);
  Q_PROPERTY(QString filePattern     READ filePattern WRITE setFilePattern);
  Q_PROPERTY(QString outputDirectory READ outputDirectory WRITE setOutputDirectory);
  Q_PROPERTY(int     darkNSummed     READ darkNSummed WRITE setDarkNSummed);
  Q_PROPERTY(int     cameraGain      READ cameraGain WRITE setCameraGain);
  Q_PROPERTY(int     cameraMode      READ cameraMode WRITE setCameraMode);
  Q_PROPERTY(int     frameSyncMode   READ frameSyncMode WRITE setFrameSyncMode);
  Q_PROPERTY(int     timerSync       READ timerSync WRITE setTimerSync);

private:
  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireDarkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue statusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue integrationModeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue exposureFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue subframesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkSubframesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue framesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue filenameFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue directoryFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue fileIndexFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setCameraModeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setFrameSyncModeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setTimerSyncFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setCameraGainFunc(QScriptContext *context, QScriptEngine *engine);

  void acquisition();
  void acquisitionError(int n);
  void haltAcquire();

 private:
  mutable QMutex         m_Mutex;
  QScriptEngine          m_ScriptEngine;
  QxrdAcquisitionThread *m_AcquisitionThread;
  QxrdApplication       *m_Application;
  QMutex                 m_Acquiring;
  QWaitCondition         m_AcquisitionWaiting;
  QWaitCondition         m_StatusWaiting;
  int                    m_Debug;
  int                    m_Cancelling;
  int                    m_NRows;
  int                    m_NCols;
  int                    m_IntegMode;
  int                    m_NSums;
  int                    m_NFrames;
  int                    m_NBufferFrames;
  int                    m_BufferFrame;
  int                    m_CurrentSum;
  int                    m_CurrentFrame;
  int                    m_AcquiringDark;
  QxrdImageData         *m_AcquiredData;
  QVector<quint16>       m_Buffer;
  double                 m_IntTimes[8];
  int                    m_NIntTimes;
  QString                m_OutputDirectory;
  QString                m_FilePattern;
  int                    m_FileIndex;
  int                    m_CameraMode;
  int                    m_FrameSyncMode;
  int                    m_TimerSync;
  int                    m_CameraGain;
  double                 m_ExposureTime;
  int                    m_IntegrationMode;
  int                    m_NSummed;
  int                    m_DarkNSummed;
};

#endif
