/******************************************************************
*
*  $Id: qxrdpilatusplugin.cpp,v 1.2 2010/09/13 20:00:20 jennings Exp $
*
*******************************************************************/

#include "qxrdpilatusplugin.h"

QxrdPilatusPlugin::QxrdPilatusPlugin()
{
}

QString QxrdPilatusPlugin::name() const
{
  return "Pilatus Detector";
}

Q_EXPORT_PLUGIN2(qxrdpilatusplugin, QxrdPilatusPlugin);

/******************************************************************
*
*  $Log: qxrdpilatusplugin.cpp,v $
*  Revision 1.2  2010/09/13 20:00:20  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
