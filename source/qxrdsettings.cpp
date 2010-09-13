/******************************************************************
*
*  $Id: qxrdsettings.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#include "qxrdsettings.h"
#include <stdio.h>

QxrdSettings::QxrdSettings(QObject *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd-2",parent),
    SOURCE_IDENT("$Id: qxrdsettings.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $")
{
  QCEP_DEBUG(DEBUG_PREFS,
             printf("QxrdSettings::QxrdSettings\n");
  )
}

QxrdSettings::~QxrdSettings()
{
  QCEP_DEBUG(DEBUG_PREFS,
             printf("QxrdSettings::~QxrdSettings\n");
  )
}

/******************************************************************
*
*  $Log: qxrdsettings.cpp,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.5.4.1  2010/05/19 21:15:35  jennings
*  PE acquisition debugging
*
*  Revision 1.5  2009/08/24 16:28:14  jennings
*  *** empty log message ***
*
*  Revision 1.4  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

