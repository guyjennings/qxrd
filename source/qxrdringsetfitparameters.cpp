/******************************************************************
*
*  $Id: qxrdringsetfitparameters.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdringsetfitparameters.h"
#include <QThread>
#include <stdio.h>
#include <math.h>
#include "qxrdmutexlocker.h"

QxrdRingSetFitParameters::QxrdRingSetFitParameters(QObject *parent) :
    m_CenterX(this, "centerX", 1024),
    m_CenterY(this, "centerY", 1024),
    m_Distance(this, "distance", 1000),
    m_Energy(this, "energy", 22000),
    m_Tilt(this, "tilt", 0),
    m_Rotation(this, "rotation", 0),
    SOURCE_IDENT("$Id: qxrdringsetfitparameters.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
{
  qRegisterMetaType<QxrdFitParameterPtr>("QxrdFitParameterPtr");
  qRegisterMetaType<QxrdRingFitParametersPtr>("QxrdRingFitParametersPtr");
  qRegisterMetaType<QxrdRingSetFitParametersPtr>("QxrdRingSetFitParametersPtr");
}

QxrdRingFitParameters* QxrdRingSetFitParameters::ring(int n) const
{
  return m_Rings.value(n).data();
}

void QxrdRingSetFitParameters::writeSettings(QxrdSettings &settings, QString section)
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
      printf("NULL ring fit parameters\n");
    } else {
      settings.setArrayIndex(i);
      r -> writeSettings(settings, "");
    }
  }

  settings.endArray();
}

void QxrdRingSetFitParameters::readSettings(QxrdSettings &settings, QString section)
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
  printf("NRings = %d\n", m_Rings.size());

  foreach(QxrdRingFitParametersPtr p, m_Rings) {
    printf("tth = %g\n", p->get_TwoTheta());
  }
}

double QxrdRingSetFitParameters::ellipseR(double chi, double eccen, double a)
{
  return (1 - eccen*eccen)*a/(1 - eccen*cos(chi-get_Rotation()));
}

/******************************************************************
*
*  $Log: qxrdringsetfitparameters.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.10  2010/08/17 20:19:26  jennings
*  QxrdFitParameter readSettings and writeSettings - saves value, limits and flags
*
*  Revision 1.1.2.9  2010/08/17 19:22:09  jennings
*  More ring fitting stuff
*
*  Revision 1.1.2.8  2010/08/17 03:25:58  jennings
*  Removed eccentricity fit parameter - is implied by angle and distance
*
*  Revision 1.1.2.7  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.6  2010/08/13 21:57:36  jennings
*  A little more powder fit implementation
*
*  Revision 1.1.2.5  2010/08/09 02:45:38  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.4  2010/08/06 21:09:35  jennings
*  Initial partial implementation of powder ring fitting widget
*
*  Revision 1.1.2.3  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*  Revision 1.1.2.2  2010/08/02 21:51:03  jennings
*  Better integration of powder ring parameter objects to script system
*
*  Revision 1.1.2.1  2010/08/02 21:11:58  jennings
*  Added classes defining fitting parameters for a set of powder rings
*
*
*******************************************************************/
