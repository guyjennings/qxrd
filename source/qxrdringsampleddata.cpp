/******************************************************************
*
*  $Id: qxrdringsampleddata.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdringsampleddata.h"
#include <QVector>
#include "qxrdmutexlocker.h"
#include "qcepproperty.h"

QxrdRingSampledData::QxrdRingSampledData(QObject *parent) :
    QObject(parent)
{
}

QScriptValue QxrdRingSampledDataToScriptValue(QScriptEngine *engine, QxrdRingSampledData* const &in)
{
  return engine->newQObject(in);
}

void QxrdRingSampledDataFromScriptValue(const QScriptValue &object, QxrdRingSampledData* &out)
{
  out = qobject_cast<QxrdRingSampledData*>(object.toQObject());
}

void QxrdRingSampledData::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

  settings.beginWriteArray(section+"/rings", count());

  for (int i=0; i<count(); i++) {
    QwtDoublePoint pt = point(i);

    settings.setArrayIndex(i);
    settings.setValue("x", pt.x());
    settings.setValue("y", pt.y());
  }

  settings.endArray();
}

void QxrdRingSampledData::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);

  clear();

  int sz = settings.beginReadArray(section+"/rings");

  for (int i=0; i<sz; i++) {
    settings.setArrayIndex(i);

    QwtDoublePoint pt(settings.value("x").toDouble(), settings.value("y").toDouble());

    append(pt);
  }

  settings.endArray();
}

void QxrdRingSampledData::append(QwtDoublePoint pt)
{
  m_Points.append(pt);
}

void QxrdRingSampledData::remove(int n)
{
  m_Points.remove(n);
}

void QxrdRingSampledData::clear()
{
  m_Points.clear();
}

void QxrdRingSampledData::setPoint(int n, QwtDoublePoint pt)
{
  m_Points.replace(n, pt);
}

int  QxrdRingSampledData::count() const
{
  return m_Points.count();
}

QwtDoublePoint QxrdRingSampledData::point(int n) const
{
  return m_Points.value(n);
}

/******************************************************************
*
*  $Log: qxrdringsampleddata.cpp,v $
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
*  Revision 1.1.2.3  2010/08/10 21:24:48  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.2  2010/08/10 20:42:25  jennings
*  Initial implementation of sampled data types
*
*  Revision 1.1.2.1  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*
*******************************************************************/

