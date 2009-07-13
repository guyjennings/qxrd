/******************************************************************
*
*  $Id: qxrdacquisitionscripting.h,v 1.6 2009/07/13 23:19:37 jennings Exp $
*
*******************************************************************/

#ifndef QXRDACQUISITIONSCRIPTING_H
#define QXRDACQUISITIONSCRIPTING_H

#include "qcepmacros.h"

#include "qxrdacquisitionparameters.h"

#include <QScriptEngine>

class QxrdDataProcessor;

class QxrdAcquisitionScripting : public QxrdAcquisitionParameters
{
  Q_OBJECT;
public:
  QxrdAcquisitionScripting(QxrdDataProcessor *proc);

public slots:
  void propertyList();
  void message(QString cmd);\

protected:
  virtual void acquire() = 0;
  virtual void acquireDark() = 0;
  virtual int acquisitionStatus(double time) = 0;

private:
  mutable QMutex         m_Mutex;
  HEADER_IDENT("$Id: qxrdacquisitionscripting.h,v 1.6 2009/07/13 23:19:37 jennings Exp $");
};

#endif // QXRDACQUISITIONSCRIPTING_H

/******************************************************************
*
*  $Log: qxrdacquisitionscripting.h,v $
*  Revision 1.6  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.5  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

