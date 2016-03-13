#include "qxrdcalibrantlibrary.h"
#include "qxrdcalibrant.h"
#include "qcepmutexlocker.h"
#include <stdio.h>

QxrdCalibrantLibrary::QxrdCalibrantLibrary(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp)
  : QcepObject("calibrantLibrary", NULL),
    m_Saver(saver),
    m_Experiment(exp)
{
  int nCals = numberStandardCalibrants();

  for (int i=0; i<nCals; i++) {
    appendCalibrant(standardCalibrant(i));
  }
}

QxrdCalibrantLibrary::~QxrdCalibrantLibrary()
{
#ifndef QT_NO_DEBUG
  printf("Deleting calibrant library\n");
#endif
}

void QxrdCalibrantLibrary::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  int nstd = numberStandardCalibrants();

  int ns   = settings->beginReadArray(section+"/stdcalibrants");

  for (int i=0; i<ns; i++) {
    settings->setArrayIndex(i);

    int isUsed = settings->value("isUsed").toInt();

    m_Calibrants[i]->set_IsUsed(isUsed);
  }

  settings->endArray();

  int nc   = settings->beginReadArray(section+"/calibrants");

  while (m_Calibrants.count() > (nc + nstd)) {
    removeCalibrant();
  }

  while (m_Calibrants.count() < (nc + nstd)) {
    appendCalibrant();
  }

  for (int i=0; i<nc; i++) {
    settings->setArrayIndex(i);

    m_Calibrants[i+nstd]->readSettings(settings, "");
  }

  settings->endArray();
}

void QxrdCalibrantLibrary::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  int nstd = numberStandardCalibrants();

  settings->beginWriteArray(section+"/stdcalibrants");

  for (int i=0; i<nstd; i++) {
    settings->setArrayIndex(i);
    settings->setValue("isUsed", m_Calibrants[i]->get_IsUsed());
  }

  settings->endArray();

  int n = m_Calibrants.count() - nstd;

  settings->beginWriteArray(section+"/calibrants");

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
  return 8;
}

QxrdCalibrantPtr QxrdCalibrantLibrary::standardCalibrant(int n)
{
  QxrdCalibrantPtr res;

  switch (n) {
  case 0: // Silicon Powder
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("Si");
      res->set_Description("Silicon Powder");
      res->set_Symmetry(QxrdCalibrant::DiamondCubic);
      res->set_A(5.43123);
      res->set_B(5.43123);
      res->set_C(5.43123);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(90);
    }
    break;

  case 1: // Alumina
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("Al2O3");
      res->set_Description("Alumina Powder");
      res->set_Symmetry(QxrdCalibrant::RHexagonal);
      res->set_A(4.7590914);
      res->set_B(4.7590914);
      res->set_C(12.991779);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(120);
    }
    break;

  case 2: // NAC
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("NAC");
      res->set_Description("NAC Powder");
      res->set_Symmetry(QxrdCalibrant::BodyCenteredCubic);
      res->set_A(10.251437);
      res->set_B(10.251437);
      res->set_C(10.251437);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(90);
    }
    break;

  case 3: // LaB6
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("LaB6");
      res->set_Description("NIST SRM LaB6 660a");
      res->set_Symmetry(QxrdCalibrant::SimpleCubic);
      res->set_A(4.15692);
      res->set_B(4.15692);
      res->set_C(4.15692);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(90);
    }
    break;

  case 4: // ZnO
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("ZnO");
      res->set_Description("Zinc oxide");
      res->set_Symmetry(QxrdCalibrant::RHexagonal);
      res->set_A(3.24989);
      res->set_B(3.24989);
      res->set_C(5.20653);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(120);
    }
    break;

  case 5: // CeO2
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("CeO2");
      res->set_Description("Cerium(IV) dioxide");
      res->set_Symmetry(QxrdCalibrant::FaceCenteredCubic);
      res->set_A(5.41165);
      res->set_B(5.41165);
      res->set_C(5.41165);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(90);
    }
    break;

  case 6: // Cr2O3
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("Cr2O3");
      res->set_Description("Chromium oxide");
      res->set_Symmetry(QxrdCalibrant::RHexagonal);
      res->set_A(4.95898);
      res->set_B(4.95898);
      res->set_C(13.5959);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(120);
    }
    break;

  case 7: // Germanium Powder
    {
      res = QxrdCalibrantPtr(new QxrdCalibrant(m_Saver, m_Experiment, sharedFromThis(), n));
      res->set_Name("Ge");
      res->set_Description("Germanium Powder");
      res->set_Symmetry(QxrdCalibrant::DiamondCubic);
      res->set_A(5.65754);
      res->set_B(5.65754);
      res->set_C(5.65754);
      res->set_Alpha(90);
      res->set_Beta(90);
      res->set_Gamma(90);
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

double QxrdCalibrantLibrary::calibrantDSpacing(int n)
{
  return qQNaN();
}

double QxrdCalibrantLibrary::calibrantTTH(int n)
{
  return qQNaN();
}
