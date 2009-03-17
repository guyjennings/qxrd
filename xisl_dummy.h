#ifndef XISL_DUMMY_H
#define XISL_DUMMY_H

#include <QObject>
#include <QTimer>

class AcquisitionTimer : public QObject 
{
  Q_OBJECT;

 public:
  AcquisitionTimer();
  void start();
  void stop();
  void setmode(int m);

 public slots:
  void timeout();

 private:
  QTimer m_Timer;
  int    m_Mode;
};

#endif
