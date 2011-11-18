#include "qxrdringsetfitparameters.h"
#include <QThread>
#include <stdio.h>
#include <math.h>
#include "qxrdmutexlocker.h"
#include "qxrdapplication.h"

QxrdRingSetFitParameters::QxrdRingSetFitParameters(QObject *parent) :
    m_CenterX(this, "centerX", 1024),
    m_CenterY(this, "centerY", 1024),
    m_Distance(this, "distance", 1000),
    m_Energy(this, "energy", 22000),
    m_Tilt(this, "tilt", 0),
    m_Rotation(this, "rotation", 0)
{
  qRegisterMetaType<QxrdFitParameterPtr>("QxrdFitParameterPtr");
  qRegisterMetaType<QxrdRingFitParametersPtr>("QxrdRingFitParametersPtr");
  qRegisterMetaType<QxrdRingSetFitParametersPtr>("QxrdRingSetFitParametersPtr");
}

QxrdRingFitParameters* QxrdRingSetFitParameters::ring(int n) const
{
  return m_Rings.value(n).data();
}

void QxrdRingSetFitParameters::writeSettings(QSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  prop_CenterX()->writeSettings(settings, section);
  prop_CenterY()->writeSettings(settings, section);
  prop_Distance()->writeSettings(settings, section);
  prop_Energy()->writeSettings(settings, section);
  prop_Tilt()->writeSettings(settings, section);
  prop_Rotation()->writeSettings(settings, section);

  settings.beginWriteArray(section+"/rings", count());

  for (int i=0; i<count(); i++) {
    QxrdRingFitParameters* r = ring(i);

    if (r==NULL) {
      g_Application->printMessage("NULL ring fit parameters");
    } else {
      settings.setArrayIndex(i);
      r -> writeSettings(settings, "");
    }
  }

  settings.endArray();
}

void QxrdRingSetFitParameters::readSettings(QSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  prop_CenterX()->readSettings(settings, section);
  prop_CenterY()->readSettings(settings, section);
  prop_Distance()->readSettings(settings, section);
  prop_Energy()->readSettings(settings, section);
  prop_Tilt()->readSettings(settings, section);
  prop_Rotation()->readSettings(settings, section);

  clear();

  int sz = settings.beginReadArray(section+"/rings");

  for (int i=0; i<sz; i++) {
    append();

    QxrdRingFitParameters* r = ring(i);

    settings.setArrayIndex(i);

    r-> readSettings(settings, "");
  }

  settings.endArray();
}

QList<QxrdRingFitParameters*> QxrdRingSetFitParameters::rings() const
{
  QList<QxrdRingFitParameters*> res;

  foreach(QxrdRingFitParametersPtr p, m_Rings) {
    res.append(p.data());
  }

  return res;
}

int QxrdRingSetFitParameters::count() const
{
  return m_Rings.count();
}

void QxrdRingSetFitParameters::clear()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "clear", Qt::BlockingQueuedConnection));
  } else {
    m_Rings.clear();
  }
}

void QxrdRingSetFitParameters::append(double twoTheta)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "append", Qt::BlockingQueuedConnection, Q_ARG(double, twoTheta)));
  } else {
    m_Rings.append(QxrdRingFitParametersPtr(new QxrdRingFitParameters(this, twoTheta)));
  }
}

void QxrdRingSetFitParameters::remove(int n)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "remove", Qt::BlockingQueuedConnection, Q_ARG(int, n)));
  } else {
    m_Rings.removeAt(n);
  }
}

void QxrdRingSetFitParameters::show()
{
  g_Application->printMessage(tr("NRings = %1").arg(m_Rings.size()));

  foreach(QxrdRingFitParametersPtr p, m_Rings) {
    g_Application->printMessage(tr("tth = %1").arg(p->get_TwoTheta()));
  }
}

double QxrdRingSetFitParameters::ellipseR(double chi, double eccen, double a)
{
  return (1 - eccen*eccen)*a/(1 - eccen*cos(chi-get_Rotation()));
}
