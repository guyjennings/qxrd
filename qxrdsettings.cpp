/******************************************************************
*
*  $Id: qxrdsettings.cpp,v 1.5 2009/08/24 16:28:14 jennings Exp $
*
*******************************************************************/

#include "qxrdsettings.h"

QxrdSettings::QxrdSettings(QObject *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd-2",parent),
    SOURCE_IDENT("$Id: qxrdsettings.cpp,v 1.5 2009/08/24 16:28:14 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdsettings.cpp,v $
*  Revision 1.5  2009/08/24 16:28:14  jennings
*  *** empty log message ***
*
*  Revision 1.4  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

