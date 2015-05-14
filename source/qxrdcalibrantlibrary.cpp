#include "qxrdcalibrantlibrary.h"
#include "qxrdcalibrant.h"
#include "qxrdmutexlocker.h"

QxrdCalibrantLibrary::QxrdCalibrantLibrary(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr exp)
  : QcepObject("calibrantlibrary", NULL),
    m_Saver(saver),
    m_Experiment(exp)
{
}

QxrdCalibrantLibrary::~QxrdCalibrantLibrary()
{
}

void QxrdCalibrantLibrary::initialize(QxrdCalibrantLibraryWPtr lib)
{
  m_CalibrantLibrary = lib;

  int nCals = numberStandardCalibrants();

  for (int i=0; i<nCals; i++) {
    appendCalibrant(standardCalibrant(i));
  }
}

QxrdCalibrantLibraryWPtr QxrdCalibrantLibrary::calibrantLibraryPtr() const
{
  return m_CalibrantLibrary;
}

void QxrdCalibrantLibrary::readSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  int nstd = numberStandardCalibrants();
  int n = settings->beginReadArray(section+"/calibrants");

  while (m_Calibrants.count() > (n + nstd)) {
    removeCalibrant();
  }

  while (m_Calibrants.count() < (n + nstd)) {
    appendCalibrant();
  }

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    m_Calibrants[i+nstd]->readSettings(settings, "");
  }

  settings->endArray();
}

void QxrdCalibrantLibrary::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  settings->beginWriteArray(section+"/calibrants");

  int nstd = numberStandardCalibrants();
  int n = m_Calibrants.count() - nstd;

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);
    m_Calibrants[i+nstd]->writeSettings(settings, "");
  }

  settings->endArray();
}

void QxrdCalibrantLibrary::removeCalibrant()
{
  m_Calibrants.removeLast();
}

void QxrdCalibrantLibrary::appendCalibrant(QxrdCalibrantPtr cal)
{
  if (cal) {
    m_Calibrants.append(cal);
  } else {
    m_Calibrants.append(standardCalibrant(0));
  }
}

int QxrdCalibrantLibrary::numberStandardCalibrants()
{
  return 3;
}

QxrdCalibrantPtr QxrdCalibrantLibrary::standardCalibrant(int n)
{
  QxrdCalibrantPtr res;

  switch (n) {
  case 0:
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, m_CalibrantLibrary));
      res->set_Name("calibrant0");
    }
    break;

  case 1:
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, m_CalibrantLibrary));
      res->set_Name("calibrant1");
    }
    break;

  case 2:
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, m_CalibrantLibrary));
      res->set_Name("calibrant2");
    }
    break;
  }

  return res;
}

int QxrdCalibrantLibrary::count()
{
  return m_Calibrants.count();
}

QString QxrdCalibrantLibrary::calibrantName(int n)
{
  QxrdCalibrantPtr p(m_Calibrants.value(n));

  if (p) {
    return p->get_Name();
  } else {
    return "";
  }
}

QxrdCalibrantWPtr QxrdCalibrantLibrary::calibrant(int n)
{
  return m_Calibrants.value(n);
}
