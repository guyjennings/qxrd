#ifndef QXRDRESPONSETIMER_H
#define QXRDRESPONSETIMER_H

#include <QObject>
#include <QTimer>
#include <QTime>

class QxrdResponseTimer : public QObject
{
    Q_OBJECT
public:
    explicit QxrdResponseTimer(int interval, int allowance, QObject *parent = 0);

public slots:
  void onTimeout();

private:
  int    m_ResponseInterval;
  int    m_ResponseAllowance;
  QTimer m_ResponseTimer;
  QTime  m_ResponseTime;
};

#endif // QXRDRESPONSETIMER_H
