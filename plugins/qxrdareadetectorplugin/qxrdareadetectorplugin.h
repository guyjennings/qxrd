/******************************************************************
*
*  $Id: qxrdareadetectorplugin.h,v 1.2 2010/09/13 20:00:11 jennings Exp $
*
*******************************************************************/

#ifndef QXRDAREADETECTORPLUGIN_H
#define QXRDAREADETECTORPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdAreaDetectorPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdDetectorPluginInterface);

public:
    QxrdAreaDetectorPlugin();

    QString name() const;
};

#endif // QXRDAREADETECTORPLUGIN_H

/******************************************************************
*
*  $Log: qxrdareadetectorplugin.h,v $
*  Revision 1.2  2010/09/13 20:00:11  jennings
*  Merged
*
*  Revision 1.1.2.4  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
