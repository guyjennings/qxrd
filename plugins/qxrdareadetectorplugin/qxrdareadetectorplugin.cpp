/******************************************************************
*
*  $Id: qxrdareadetectorplugin.cpp,v 1.2 2010/09/13 20:00:11 jennings Exp $
*
*******************************************************************/

#include "qxrdareadetectorplugin.h"

QxrdAreaDetectorPlugin::QxrdAreaDetectorPlugin()
{
}

QString QxrdAreaDetectorPlugin::name() const
{
  return "EPICS Area Detector Plugin";
}

Q_EXPORT_PLUGIN2(qxrdareadetectorplugin, QxrdAreaDetectorPlugin);

/******************************************************************
*
*  $Log: qxrdareadetectorplugin.cpp,v $
*  Revision 1.2  2010/09/13 20:00:11  jennings
*  Merged
*
*  Revision 1.1.2.4  2010/07/23 19:24:44  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.3  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
