/******************************************************************
*
*  $Id: qxrdprocessorinterface.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPROCESSORINTERFACE_H
#define QXRDPROCESSORINTERFACE_H

#include <QtPlugin>
#include "qxrdnamedplugininterface.h"

class QxrdProcessorInterface : public QxrdNamedPluginInterface
{
public:
  virtual ~QxrdProcessorInterface() {}
};

Q_DECLARE_INTERFACE(QxrdProcessorInterface, "gov.anl.aps.cep.Qxrd.ProcessorInterface/1.0");

#endif // QXRDPROCESSORINTERFACE_H

/******************************************************************
*
*  $Log: qxrdprocessorinterface.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
