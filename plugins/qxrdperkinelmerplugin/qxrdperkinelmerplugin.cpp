/******************************************************************
*
*  $Id: qxrdperkinelmerplugin.cpp,v 1.2 2010/09/13 20:00:17 jennings Exp $
*
*******************************************************************/

#include "qxrdperkinelmerplugin.h"

QxrdPerkinElmerPlugin::QxrdPerkinElmerPlugin()
{
}

QString QxrdPerkinElmerPlugin::name() const
{
  return "Perkin Elmer XRD Detector";
}

Q_EXPORT_PLUGIN2(qxrdperkinelmerplugin, QxrdPerkinElmerPlugin);

/******************************************************************
*
*  $Log: qxrdperkinelmerplugin.cpp,v $
*  Revision 1.2  2010/09/13 20:00:17  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
