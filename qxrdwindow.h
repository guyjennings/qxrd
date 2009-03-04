#ifndef QXRDWINDOW_H
#define QXRDWINDOW_H

#include <QMainWindow>
#include "ui_qxrdwindow.h"

class QxrdApplication;
class QxrdAcquisitionThread;

class QxrdWindow : public QMainWindow, public Ui::QxrdWindow
{
  Q_OBJECT;

 public:
  QxrdWindow(QxrdApplication *app, QWidget *parent=0);

 public slots:
  void setAcquisitionThread(QxrdAcquisitionThread *acq);

  void loadData();
  void saveData();
  
  void printMessage(QString msg);

  void setCancelButton();
  void setAcquireButton();
  void setIntegrationTime(int n, double t);

  void setExposureTime(double t);
  void setIntegrationMode(int mode);
  void setNSummed(int nsummed);
  void setNFrames(int nframes);
  void setFileIndex(int index);
  void setFilePattern(QString pattern);

  void acquiredFrame(int isum, int nsum, int iframe, int nframe);

 public:
  double  exposureTime();
  int     integrationMode();
  int     nSummed();
  int     nFrames();
  int     fileIndex();
  QString filePattern();

 private:
  QxrdApplication        *m_Application;
  QxrdAcquisitionThread  *m_AcquisitionThread;
  QVector<double>         m_Exposures;
};

#endif
