#ifndef QXRDACQUISITIONPERKINELMER_H
#define QXRDACQUISITIONPERKINELMER_H

#include "qxrdacquisitionoperations.h"

#include <QMutex>
#include <QWaitCondition>
#include <QVector>

class QxrdAcquisitionThread;

class QxrdAcquisitionPerkinElmer : public QxrdAcquisitionOperations
{
  Q_OBJECT;
public:
  QxrdAcquisitionPerkinElmer(QxrdAcquisitionThread *thread);
  ~QxrdAcquisitionPerkinElmer();
  typedef QxrdAcquisitionOperations inherited;

public slots:
  void acquire();
  void acquireDark();
  void cancel();
  void cancelDark();

signals:
  void acquireStarted(int dark);
  void acquireComplete(int dark);
  void acquiredFrame(QString fileName, int index, int isum, int nsum, int iframe, int nframe);

public:
  void initialize();
//  bool onEndFrame();
  void onEndAcquisition();
  QVector<double> readoutTimes();
  double  readoutTime() const;
  int acquisitionStatus(double time);
  void onEndFrameCallback();

protected:
  void acquisition(int isDark);
  void haltAcquire();

private slots:
  void onEndFrame();

private:
  void acquisitionError(int n);
  void acquisitionInitError(int n);
  void acquisitionNSensorsError(int n);

private:
  mutable QMutex         m_Mutex;
  QMutex                 m_Acquiring;
  QWaitCondition         m_StatusWaiting;
  int                    m_Cancelling;
  int                    m_AcquireDark;
  int                    m_NRows;
  int                    m_NCols;
  int                    m_ExposuresToSum;
  int                    m_FilesInSequence;
  int                    m_CurrentExposure;
  int                    m_CurrentFile;
  int                    m_BufferSize;
  int                    m_BufferIndex;
  QVector<quint16>       m_Buffer;
  QxrdImageData         *m_AcquiredData;
  QVector<double>        m_ReadoutTimes;
};

#endif // QXRDACQUISITIONPERKINELMER_H
