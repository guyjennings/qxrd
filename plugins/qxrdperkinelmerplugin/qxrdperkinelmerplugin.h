/******************************************************************
*
*  $Id: qxrdperkinelmerplugin.h,v 1.2 2010/09/13 20:00:17 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPERKINELMERPLUGIN_H
#define QXRDPERKINELMERPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdPerkinElmerPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdDetectorPluginInterface);

public:
    QxrdPerkinElmerPlugin();

    QString name() const;
};

#endif // QXRDPERKINELMERPLUGIN_H

/******************************************************************
*
*  $Log: qxrdperkinelmerplugin.h,v $
*  Revision 1.2  2010/09/13 20:00:17  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
