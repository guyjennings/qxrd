/******************************************************************
*
*  $Id: qxrdringsampleddata.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDRINGSAMPLEDDATA_H
#define QXRDRINGSAMPLEDDATA_H

#include <QObject>
#include <QMutex>
#include "qcepmacros.h"
#include <QSharedPointer>
#include "qwt_double_rect.h"
#include "qxrdsettings.h"
#include <QScriptEngine>

class QxrdRingSampledData : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdRingSampledData(QObject *parent = 0);

signals:

public slots:
  void append(QwtDoublePoint pt);
  void remove(int n);
  void clear();
  void setPoint(int n, QwtDoublePoint pt);
  int  count() const;
  QwtDoublePoint point(int n) const;

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

private:
  mutable QMutex         m_Mutex;

  QVector<QwtDoublePoint> m_Points;

  HEADER_IDENT("$Id: qxrdringsampleddata.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

//typedef QSharedPointer<QxrdRingSampledData> QxrdRingSampledDataPtr;
typedef QxrdRingSampledData *QxrdRingSampledDataPtr;

Q_DECLARE_METATYPE(QxrdRingSampledData*);

QScriptValue QxrdRingSampledDataToScriptValue(QScriptEngine *engine, QxrdRingSampledData* const &in);
void QxrdRingSampledDataFromScriptValue(const QScriptValue &object, QxrdRingSampledData* &out);

#endif // QXRDRINGSAMPLEDDATA_H

/******************************************************************
*
*  $Log: qxrdringsampleddata.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.6  2010/08/17 20:19:26  jennings
*  QxrdFitParameter readSettings and writeSettings - saves value, limits and flags
*
*  Revision 1.1.2.5  2010/08/17 19:22:09  jennings
*  More ring fitting stuff
*
*  Revision 1.1.2.4  2010/08/16 21:45:27  jennings
*  Distinguish between initial and refined fit parameters and data in powder ring fitting
*  Add read/write settings routines for powder ring fitting data
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

