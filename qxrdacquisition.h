#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include <QObject>
#include <QMutex>
#include <QVector>
#include <QFuture>

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"

class QxrdApplication;
class QxrdAcquisitionThread;
class QxrdWindow;

class QxrdAcquisition : public QObject
{
  Q_OBJECT;

 public:
  QxrdAcquisition(QxrdApplication *app, QxrdWindow *win, QxrdAcquisitionThread *thread);
  ~QxrdAcquisition();

 public slots:
  void initialize();
  void acquire(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum, int nframes);
  void acquireDark(QString outDir, QString filePattern, int fileIndex, int integmode, int nsum);
  void resultsAvailable(int chan);
  void savingComplete(int chan);
  void cancel();
  void cancelDark();

 private slots:
  void _haltAcquire();

 signals:
  void newDataAvailable();
  void resultsChanged();
  void acquireComplete();
  void haltAcquire();
  void printMessage(QString msg);
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);
  void fileIndexChanged(int index);
  void statusMessage(QString msg);
  void summedFrameCompleted(QString fileName, int iframe);

 public:
  void onEndFrame();
  void onEndAcquisition();
  QVector<double> integrationTimes();
  int acquisitionStatus();
  void setWindow(QxrdWindow *win);
  QxrdImageData *nextAvailableImage();
  void enqueue(QxrdImageData *img);

 private:
  void acquisitionError(int n);

 private:
  QxrdAcquisitionThread *m_Thread;
  QMutex                 m_Mutex;
  QxrdApplication       *m_Application;
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
  QVector<unsigned short> m_Buffer;
  QVector< QFuture<int> > m_Saved;
  double                 m_IntTimes[8];
  int                    m_NIntTimes;
  QString                m_OutputDir;
  QString                m_FilePattern;
  int                    m_FileIndex;
  QxrdImageQueue         m_AvailableImages;
  QxrdWindow            *m_Window;
};

#endif
