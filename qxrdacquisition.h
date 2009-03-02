#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include <QObject>
#include <QMutex>

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
  void acquire(double integ, int nsum, int nframes);
  void resultsAvailable(int chan);
  void savingComplete(int chan);
  void cancel();

 public:
 signals:
  void newDataAvailable();
  void resultsChanged();
  void acquireComplete();
  void printMessage(QString msg);

 private:
  void acquisitionError(int n);

 private:
  QxrdAcquisitionThread *m_Thread;
  QMutex                 m_Mutex;
  QxrdApplication       *m_Application;
  int                    m_Cancel;
};

#endif
