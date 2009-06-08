#include "qxrdacquisitionparameters.h"

#include <QMutexLocker>
#include "qxrdsettings.h"
#include <QMetaProperty>
#include <QStringList>

QxrdAcquisitionParameters::QxrdAcquisitionParameters()
  : inherited(),
    m_Mutex(QMutex::Recursive),
    m_Debug(0),
    m_OutputDirectory(""),
    m_FilePattern(""),
    m_FileIndex(0),
    // m_CameraMode(0),
    // m_FrameSyncMode(0),
    // m_TimerSync(0),
    m_CameraGain(0),
    m_ExposureTime(0),
    m_ReadoutMode(0),
    m_SummedExposures(1),
    m_DarkSummedExposures(1),
    m_FilesInSequence(1)
{
}

void QxrdAcquisitionParameters::saveSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  const QMetaObject *meta = &staticMetaObject;

  int count = meta->propertyCount();
  int offset = meta->propertyOffset();

  settings -> beginGroup(section);

  for (int i=offset; i<count; i++) {
    QMetaProperty metaproperty = meta->property(i);

    const char *name = metaproperty.name();
    QVariant value = property(name);

    settings -> setValue(name, value);
  }

  settings -> endGroup();
}

void QxrdAcquisitionParameters::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  settings -> beginGroup(section);

  QStringList keys = settings -> childKeys();

  foreach (QString key, keys) {
    QVariant val = property(qPrintable(key));

    if (val.isValid()) {
      setProperty(qPrintable(key), settings -> value(key));
    }
  }

  settings -> endGroup();
}

void QxrdAcquisitionParameters::setCameraGain(int mode)
{
  QMutexLocker lock(&m_Mutex);

  m_CameraGain = mode;
}

int QxrdAcquisitionParameters::cameraGain() const
{
  QMutexLocker lock(&m_Mutex);

  return m_CameraGain;
}

void QxrdAcquisitionParameters::setExposureTime(double t)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setExposureTime(%1->%2)")
                      .arg(m_ExposureTime).arg(t));
  }

  if (m_ExposureTime != t) {
    m_ExposureTime = t;

    emit exposureTimeChanged(t);
  }
}

void QxrdAcquisitionParameters::setReadoutMode(int mode)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setReadoutMode(%1->%2)")
                      .arg(m_ReadoutMode).arg(mode));
  }

  if (m_ReadoutMode != mode) {
    m_ReadoutMode = mode;

    emit readoutModeChanged(mode);
  }
}

void QxrdAcquisitionParameters::setSummedExposures(int nsummed)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setSummedExposures(%1->%2)")
                      .arg(m_SummedExposures).arg(nsummed));
  }

  if (m_SummedExposures != nsummed) {
    m_SummedExposures = nsummed;

    emit summedExposuresChanged(nsummed);
  }
}

void QxrdAcquisitionParameters::setFilesInSequence(int nframes)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setFilesInSequence(%1->%2)")
                      .arg(m_FilesInSequence).arg(nframes));
  }

  if (m_FilesInSequence != nframes) {
    m_FilesInSequence = nframes;

    emit filesInSequenceChanged(nframes);
  }
}

void QxrdAcquisitionParameters::setFileIndex(int index)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setFileIndex(%1->%2)")
                      .arg(m_FileIndex).arg(index));
  }

  if (m_FileIndex != index) {
    m_FileIndex = index;

    emit fileIndexChanged(index);
  }
}

void QxrdAcquisitionParameters::setFilePattern(QString pattern)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setFilePattern(%1->%2)")
                      .arg(m_FilePattern).arg(pattern));
  }

  if (m_FilePattern != pattern) {
    m_FilePattern = pattern;

    emit filePatternChanged(pattern);
  }
}

void QxrdAcquisitionParameters::setOutputDirectory(QString path)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setOutputDirectory(%1->%2)")
                      .arg(m_OutputDirectory).arg(path));
  }

  if (m_OutputDirectory != path) {
    m_OutputDirectory = path;

    emit outputDirectoryChanged(path);
  }
}

void QxrdAcquisitionParameters::setDarkSummedExposures(int nsummed)
{
  QMutexLocker lock(&m_Mutex);

  if (m_Debug) {
    emit printMessage(tr("QxrdAcquisitionParameters::setDarkSummedExposures(%1->%2)")
                      .arg(m_DarkSummedExposures).arg(nsummed));
  }

  if (m_DarkSummedExposures != nsummed) {
    m_DarkSummedExposures = nsummed;

    emit darkSummedExposuresChanged(nsummed);
  }
}

double  QxrdAcquisitionParameters::exposureTime() const
{
  QMutexLocker lock(&m_Mutex);

  return m_ExposureTime;
}

int     QxrdAcquisitionParameters::readoutMode() const
{
  QMutexLocker lock(&m_Mutex);

  return m_ReadoutMode;
}

int     QxrdAcquisitionParameters::summedExposures() const
{
  QMutexLocker lock(&m_Mutex);

  return m_SummedExposures;
}

int     QxrdAcquisitionParameters::filesInSequence() const
{
  QMutexLocker lock(&m_Mutex);

  return m_FilesInSequence;
}

int     QxrdAcquisitionParameters::fileIndex() const
{
  QMutexLocker lock(&m_Mutex);

  return m_FileIndex;
}

QString QxrdAcquisitionParameters::filePattern() const
{
  QMutexLocker lock(&m_Mutex);

  return m_FilePattern;
}

QString QxrdAcquisitionParameters::outputDirectory() const
{
  QMutexLocker lock(&m_Mutex);

  return m_OutputDirectory;
}

int     QxrdAcquisitionParameters::darkSummedExposures() const
{
  QMutexLocker lock(&m_Mutex);

  return m_DarkSummedExposures;
}

int     QxrdAcquisitionParameters::debug() const
{
  QMutexLocker lock(&m_Mutex);

  return m_Debug;
}

void QxrdAcquisitionParameters::setDebug(int dbg)
{
  QMutexLocker lock(&m_Mutex);

  m_Debug = dbg;
}

