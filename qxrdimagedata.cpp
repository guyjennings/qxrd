#include "qxrdimagedata.h"

#include <QDir>
#include <QFileInfo>

QxrdImageData::QxrdImageData(int width, int height)
  : QcepImageData<double>(width, height),
    m_Lock(QReadWriteLock::Recursive),
    m_ReadoutMode(0),
    m_ExposureTime(0),
    m_SummedExposures(0),
    m_ImageNumber(0)
{
}

int QxrdImageData::readoutMode()
{
  return m_ReadoutMode;
}

void QxrdImageData::setReadoutMode(int mode)
{
  m_ReadoutMode = mode;
}

double QxrdImageData::exposureTime() const
{
  return m_ExposureTime;
}

void QxrdImageData::setExposureTime(double t)
{
  m_ExposureTime = t;
}

int QxrdImageData::summedExposures()
{
  return m_SummedExposures;
}

void QxrdImageData::setSummedExposures(int n)
{
  m_SummedExposures = n;
}

QReadWriteLock *QxrdImageData::rwLock()
{
  return &m_Lock;
}

int QxrdImageData::imageNumber()
{
  return m_ImageNumber;
}

void QxrdImageData::setImageNumber(int n)
{
  m_ImageNumber = n;
}

QString QxrdImageData::rawFileName()
{
  QFileInfo info(filename());

  QString name = info.dir().filePath(
      info.completeBaseName()+".raw.tif");

  return name;
}
