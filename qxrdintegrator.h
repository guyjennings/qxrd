/******************************************************************
*
*  $Id: qxrdintegrator.h,v 1.3 2009/07/08 19:06:27 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include <QObject>
#include "qcepproperty.h"
#include "qxrdsettings.h"

class QxrdIntegrator : public QObject
{
  Q_OBJECT;

public:
  QxrdIntegrator(QObject *parent=0);

  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

private:
  mutable QMutex            m_Mutex;

  HEADER_IDENT("$Id: qxrdintegrator.h,v 1.3 2009/07/08 19:06:27 jennings Exp $");
};

#endif // QXRDINTEGRATOR_H

/******************************************************************
*
*  $Log: qxrdintegrator.h,v $
*  Revision 1.3  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

