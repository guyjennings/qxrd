/******************************************************************
*
*  $Id: qxrdintegrator.cpp,v 1.3 2009/07/08 19:06:27 jennings Exp $
*
*******************************************************************/

#include "qxrdintegrator.h"

QxrdIntegrator::QxrdIntegrator(QObject *parent)
  : QObject(parent),
    SOURCE_IDENT("$Id: qxrdintegrator.cpp,v 1.3 2009/07/08 19:06:27 jennings Exp $")
{
}

void QxrdIntegrator::writeSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdIntegrator::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

/******************************************************************
*
*  $Log: qxrdintegrator.cpp,v $
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

