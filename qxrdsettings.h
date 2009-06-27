/******************************************************************
*
*  $Id: qxrdsettings.h,v 1.3 2009/06/27 22:50:33 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSETTINGS_H
#define QXRDSETTINGS_H

#include "qcepmacros.h"

#include <QSettings>

class QxrdSettings : public QSettings
{
  Q_OBJECT;

public:
  QxrdSettings(QObject *parent=0);

private:
  HEADER_IDENT("$Id: qxrdsettings.h,v 1.3 2009/06/27 22:50:33 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdsettings.h,v $
*  Revision 1.3  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

