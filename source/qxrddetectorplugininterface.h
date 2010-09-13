/******************************************************************
*
*  $Id: qxrddetectorplugininterface.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDETECTORPLUGININTERFACE_H
#define QXRDDETECTORPLUGININTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdDetectorPluginInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdDetectorPluginInterface() {}
};

Q_DECLARE_INTERFACE(QxrdDetectorPluginInterface, "gov.anl.aps.cep.Qxrd.DetectorInterface/1.0");

#endif // QXRDDETECTORPLUGININTERFACE_H

/******************************************************************
*
*  $Log: qxrddetectorplugininterface.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
