/******************************************************************
*
*  $Id: qxrdringsetfitparameters.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRINGSETFITPARAMETERS_H
#define QXRDRINGSETFITPARAMETERS_H

#include "qxrdfitparameter.h"
#include "qxrdringfitparameters.h"
#include "qxrdsettings.h"

class QxrdRingSetFitParameters : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdRingSetFitParameters(QObject *parent = 0);

public:
  Q_PROPERTY(double centerX READ get_CenterX WRITE set_CenterX);
  QXRD_FIT_PARAMETER(CenterX);

  Q_PROPERTY(double centerY READ get_CenterY WRITE set_CenterY);
  QXRD_FIT_PARAMETER(CenterY);

  Q_PROPERTY(double distance READ get_Distance WRITE set_Distance);
  QXRD_FIT_PARAMETER(Distance);

  Q_PROPERTY(double energy READ get_Energy WRITE set_Energy);
  QXRD_FIT_PARAMETER(Energy);

  Q_PROPERTY(double tilt READ get_Tilt WRITE set_Tilt);
  QXRD_FIT_PARAMETER(Tilt);

  Q_PROPERTY(double rotation READ get_Rotation WRITE set_Rotation);
  QXRD_FIT_PARAMETER(Rotation);

public slots:
  QList<QxrdRingFitParameters*> rings() const;
  QxrdRingFitParameters* ring(int n) const;
  int count() const;
  void clear();
  void append(double twoTheta=0);
  void remove(int n);
  void show();

  double ellipseR(double chi, double eccen, double a);

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

private:
  mutable QMutex         m_Mutex;

  QList<QxrdRingFitParametersPtr> m_Rings;

  HEADER_IDENT("$Id: qxrdringsetfitparameters.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

typedef QxrdRingSetFitParameters *QxrdRingSetFitParametersPtr;

Q_DECLARE_METATYPE(QxrdRingSetFitParametersPtr);

#endif // QXRDRINGSETFITPARAMETERS_H

/******************************************************************
*
*  $Log: qxrdringsetfitparameters.h,v $
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
