/******************************************************************
*
*  $Id: qxrdringfitparameters.h,v 1.3 2010/10/21 16:31:24 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRINGFITPARAMETERS_H
#define QXRDRINGFITPARAMETERS_H

#include "qxrdfitparameter.h"
#include <QScriptEngine>
#include "qxrdsettings.h"

class QxrdRingSetFitParameters;

class QxrdRingFitParameters : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdRingFitParameters(QxrdRingSetFitParameters *ringSet = 0, double twoTheta=0);

public:
  Q_PROPERTY(double twoTheta READ get_TwoTheta WRITE set_TwoTheta);
  QXRD_FIT_PARAMETER(TwoTheta);

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

private:
  mutable QMutex         m_Mutex;

  QxrdRingSetFitParameters *m_RingSet;

  HEADER_IDENT("$Id: qxrdringfitparameters.h,v 1.3 2010/10/21 16:31:24 jennings Exp $");
};

typedef QSharedPointer<QxrdRingFitParameters> QxrdRingFitParametersPtr;
//typedef QxrdRingFitParameters *QxrdRingFitParametersPtr;

Q_DECLARE_METATYPE(QxrdRingFitParameters*);

QScriptValue QxrdRingFitToScriptValue(QScriptEngine *engine, QxrdRingFitParameters* const &in);
void QxrdRingFitFromScriptValue(const QScriptValue &object, QxrdRingFitParameters* &out);

#endif // QXRDRINGFITPARAMETERS_H

/******************************************************************
*
*  $Log: qxrdringfitparameters.h,v $
*  Revision 1.3  2010/10/21 16:31:24  jennings
*  Implemented saving of settings soon after they change, rather than at program exit
*
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
