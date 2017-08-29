#include "qxrdresponsetimer.h"
#include <stdlib.h>
#include "qxrdapplication.h"

QxrdResponseTimer::QxrdResponseTimer(int interval, int allowance,  QcepObject *parent) :
  QObject(parent),
  m_ResponseInterval(interval),
  m_ResponseAllowance(allowance)
{
  connect(&m_ResponseTimer, &QTimer::timeout, this, &QxrdResponseTimer::onTimeout);

  m_ResponseTime.start();
  m_ResponseTimer.start(m_ResponseInterval);
}

void QxrdResponseTimer::onTimeout()
{
  int elapsedTime = m_ResponseTime.restart();

  if (g_Application && abs(elapsedTime-m_ResponseInterval) > m_ResponseAllowance) {
    g_Application->printMessage(tr("Excessive response time %1").arg(elapsedTime-m_ResponseInterval));
  }
}
