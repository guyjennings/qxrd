#ifndef QXRDRESPONSETIMER_H
#define QXRDRESPONSETIMER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include <QTimer>
#include <QTime>

class QXRD_EXPORT QxrdResponseTimer : public QObject
{
    Q_OBJECT
public:
    explicit QxrdResponseTimer(int interval, int allowance, QcepObject *parent);

public slots:
  void onTimeout();

private:
  int    m_ResponseInterval;
  int    m_ResponseAllowance;
  QTimer m_ResponseTimer;
  QTime  m_ResponseTime;
};

#endif // QXRDRESPONSETIMER_H
