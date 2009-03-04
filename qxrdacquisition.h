#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include <QObject>
#include <QMutex>
#include <QVector>

class QxrdApplication;
class QxrdAcquisitionThread;

class QxrdAcquisition : public QObject
{
  Q_OBJECT;

 public:
  QxrdAcquisition(QxrdApplication *app, QxrdAcquisitionThread *thread);
  ~QxrdAcquisition();

 public slots:
  void initialize();
  void acquire(int integmode, int nsum, int nframes);
  void resultsAvailable(int chan);
  void savingComplete(int chan);
  void cancel();
  void saveData(QString name);
  void readSettings();
  void saveSettings();

 private slots:
  void _haltAcquire();

 signals:
  void newDataAvailable();
  void resultsChanged();
  void acquireComplete();
  void haltAcquire();
  void printMessage(QString msg);
  void acquiredFrame(int isum, int nsum, int iframe, int nframe);

 public:
  void onEndFrame();
  void onEndAcquisition();
  QVector<double> integrationTimes();

 private:
  void acquisitionError(int n);

 private:
  QxrdAcquisitionThread *m_Thread;
  QMutex                 m_Mutex;
  QxrdApplication       *m_Application;
//  int                    m_Cancel;
  int                    m_NRows;
  int                    m_NCols;
  int                    m_IntegMode;
  int                    m_NSums;
  int                    m_NFrames;
  int                    m_NBufferFrames;
  int                    m_BufferFrame;
  int                    m_CurrentSum;
  int                    m_CurrentFrame;
  QVector<double>        m_AcquiredImage;
  QVector<unsigned short> m_Buffer;
  double                 m_IntTimes[8];
  int                    m_NIntTimes;
  QString                m_FilePattern;
  int                    m_FileIndex;
};

#endif
