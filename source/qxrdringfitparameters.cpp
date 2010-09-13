/******************************************************************
*
*  $Id: qxrdringfitparameters.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdringfitparameters.h"
#include "qxrdringsetfitparameters.h"
#include "qxrdmutexlocker.h"

QxrdRingFitParameters::QxrdRingFitParameters(QxrdRingSetFitParameters *ringSet, double twoTheta) :
    QObject(ringSet),
    m_TwoTheta(this, "twoTheta", twoTheta),
    m_RingSet(ringSet)
{
}

QScriptValue QxrdRingFitToScriptValue(QScriptEngine *engine, QxrdRingFitParameters* const &in)
{
  return engine->newQObject(in);
}

void QxrdRingFitFromScriptValue(const QScriptValue &object, QxrdRingFitParameters* &out)
{
  out = qobject_cast<QxrdRingFitParameters*>(object.toQObject());
}

void QxrdRingFitParameters::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  prop_TwoTheta()->writeSettings(settings, section);
}

void QxrdRingFitParameters::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  prop_TwoTheta()->readSettings(settings, section);
}

/******************************************************************
*
*  $Log: qxrdringfitparameters.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.6  2010/08/17 20:19:26  jennings
*  QxrdFitParameter readSettings and writeSettings - saves value, limits and flags
*
*  Revision 1.1.2.5  2010/08/17 03:25:58  jennings
*  Removed eccentricity fit parameter - is implied by angle and distance
*
*  Revision 1.1.2.4  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
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
