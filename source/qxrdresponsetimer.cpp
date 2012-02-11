#include "qxrdresponsetimer.h"
#include <stdlib.h>
#include "qxrdapplication.h"

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

  if (g_Application && abs(elapsedTime-m_ResponseInterval) > 1000) {
    g_Application->printMessage(tr("Excessive response time %1").arg(elapsedTime-m_ResponseInterval));
  }
}
