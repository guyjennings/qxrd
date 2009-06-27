/******************************************************************
*
*  $Id: qxrdintegrator.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATOR_H
#define QXRDINTEGRATOR_H

#include "qcepmacros.h"

#include <QObject>

class QxrdIntegrator : public QObject
{
  Q_OBJECT;

public:
  QxrdIntegrator(QObject *parent=0);

private:
  HEADER_IDENT("$Id: qxrdintegrator.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDINTEGRATOR_H

/******************************************************************
*
*  $Log: qxrdintegrator.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

