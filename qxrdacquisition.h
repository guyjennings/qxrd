#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include <QObject>
#include <QMutex>

class QxrdApplication;

class QxrdAcquisition : public QObject
{
  Q_OBJECT;

 public:
  QxrdAcquisition(QxrdApplication *app);
  ~QxrdAcquisition();

 public slots:
  void acquire(double integ, int nreps);
  void resultsAvailable(int chan);
  void savingComplete(int chan);

 public:
 signals:
  void newDataAvailable();
  void resultsChanged();

 private:
  QMutex            m_Mutex;
  QxrdApplication  *m_Application;
};

#endif
