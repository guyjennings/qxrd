/******************************************************************
*
*  $Id: qxrdringsetsampleddata.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRINGSETSAMPLEDDATA_H
#define QXRDRINGSETSAMPLEDDATA_H

#include <QObject>
#include "qcepmacros.h"
#include "qxrdringsampleddata.h"
//#include "qxrdringsetfitparameters.h"
#include "qxrdsettings.h"

class QxrdRingSetSampledData : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdRingSetSampledData(/*QxrdRingSetFitParametersPtr parms, */QObject *parent=0);

public slots:
  int count() const;
  QxrdRingSampledData* ring(int n) const;
  void setRing(int n, QxrdRingSampledDataPtr d);
  void append();
  void append(QxrdRingSampledDataPtr d);
  void remove(int n);
  void clear();

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

private:
  mutable QMutex         m_Mutex;

//  QxrdRingSetFitParametersPtr m_FitParms;

  QList<QxrdRingSampledDataPtr> m_Rings;

  HEADER_IDENT("$Id: qxrdringsetsampleddata.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

typedef QxrdRingSetSampledData *QxrdRingSetSampledDataPtr;

Q_DECLARE_METATYPE(QxrdRingSetSampledDataPtr);

#endif // QXRDRINGSETSAMPLEDDATA_H

/******************************************************************
*
*  $Log: qxrdringsetsampleddata.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.7  2010/08/17 20:19:26  jennings
*  QxrdFitParameter readSettings and writeSettings - saves value, limits and flags
*
*  Revision 1.1.2.6  2010/08/17 19:22:09  jennings
*  More ring fitting stuff
*
*  Revision 1.1.2.5  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
*
*  Revision 1.1.2.4  2010/08/13 21:57:36  jennings
*  A little more powder fit implementation
*
*  Revision 1.1.2.3  2010/08/10 20:42:25  jennings
*  Initial implementation of sampled data types
*
*  Revision 1.1.2.2  2010/08/03 20:14:38  jennings
*  Added classes for resampling ring centroids and fitting ring parameters
*
*  Revision 1.1.2.1  2010/08/03 19:24:19  jennings
*  Added data types to hold sampled powder ring coordinates
*
*
*******************************************************************/

