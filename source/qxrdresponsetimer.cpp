#include "qxrdresponsetimer.h"
#include <stdlib.h>

QxrdResponseTimer::QxrdResponseTimer(int interval,  QObject *parent) :
  QObject(parent),
  m_ResponseInterval(interval)
{
  connect(&m_ResponseTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

  m_ResponseTime.start();
  m_ResponseTimer.start(m_ResponseInterval);
}

void QxrdResponseTimer::onTimeout()
{
  int elapsedTime = m_ResponseTime.restart();

  if (abs(elapsedTime-m_ResponseInterval) > 1000) {
    emit printMessage(QDateTime::currentDateTime(),
                      tr("Excessive response time %1").arg(elapsedTime-m_ResponseInterval));
  }
}
