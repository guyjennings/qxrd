#include "qxrdimagedata.h"

QxrdImageData::QxrdImageData(int width, int height)
  : QcepImageData<double>(width, height),
    m_IntegrationMode(0),
    m_NSummed(0)
{
}

int QxrdImageData::integrationMode()
{
  return m_IntegrationMode;
}

void QxrdImageData::setIntegrationMode(int mode)
{
  m_IntegrationMode = mode;
}

int QxrdImageData::nSummed()
{
  return m_NSummed;
}

void QxrdImageData::setNSummed(int n)
{
  m_NSummed = n;
}

QReadWriteLock *QxrdImageData::rwLock()
{
  return &m_Lock;
}
