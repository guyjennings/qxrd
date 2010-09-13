/******************************************************************
*
*  $Id: qxrdringsetsampleddata.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdringsetsampleddata.h"
#include "qxrdmutexlocker.h"
#include "qcepproperty.h"
#include <stdio.h>
#include <QThread>

QxrdRingSetSampledData::QxrdRingSetSampledData(/*QxrdRingSetFitParametersPtr parms,*/ QObject *parent) :
    QObject(parent)/*,
    m_FitParms(parms)*/
{
}

void QxrdRingSetSampledData::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  settings.beginWriteArray(section+"/rings", count());

  for (int i=0; i<count(); i++) {
    QxrdRingSampledDataPtr r = ring(i);

    if (r==NULL) {
      printf("NULL ring sampled data\n");
    } else {
      settings.setArrayIndex(i);
      r -> writeSettings(settings, "");
    }
  }

  settings.endArray();
}

void QxrdRingSetSampledData::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  clear();

  int sz = settings.beginReadArray(section+"/rings");

  for (int i=0; i<sz; i++) {
    append();

    QxrdRingSampledDataPtr r = ring(i);

    settings.setArrayIndex(i);

    r-> readSettings(settings, "");
  }

  settings.endArray();
}

void QxrdRingSetSampledData::clear()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "clear", Qt::BlockingQueuedConnection));
  } else {
    m_Rings.clear();
  }
}

int QxrdRingSetSampledData::count() const
{
  return m_Rings.count();
}

QxrdRingSampledData* QxrdRingSetSampledData::ring(int n) const
{
  return m_Rings.value(n);
}

void QxrdRingSetSampledData::setRing(int n, QxrdRingSampledDataPtr d)
{
  m_Rings.replace(n, d);
}

void QxrdRingSetSampledData::append()
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this, "append", Qt::BlockingQueuedConnection));
  } else {
    m_Rings.append(QxrdRingSampledDataPtr(new QxrdRingSampledData(this)));
  }
}

void QxrdRingSetSampledData::append(QxrdRingSampledDataPtr d)
{
  if (d) {
    m_Rings.append(d);
  } else {
    m_Rings.append(QxrdRingSampledDataPtr(new QxrdRingSampledData(this)));
  }
}

void QxrdRingSetSampledData::remove(int n)
{
  m_Rings.removeAt(n);
}

/******************************************************************
*
*  $Log: qxrdringsetsampleddata.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.5  2010/08/17 19:22:09  jennings
*  More ring fitting stuff
*
*  Revision 1.1.2.4  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.3  2010/08/13 21:57:36  jennings
*  A little more powder fit implementation
*
*  Revision 1.1.2.2  2010/08/10 20:42:25  jennings
*  Initial implementation of sampled data types
*
*  Revision 1.1.2.1  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*
*******************************************************************/

