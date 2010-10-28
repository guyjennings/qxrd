/******************************************************************
*
*  $Id: qxrdpilatusplugin.h,v 1.2 2010/09/13 20:00:20 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPILATUSPLUGIN_H
#define QXRDPILATUSPLUGIN_H

#include <QObject>
#include "qxrddetectorplugininterface.h"

class QxrdPilatusPlugin : public QObject, public QxrdDetectorPluginInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdDetectorPluginInterface);

public:
    QxrdPilatusPlugin();

    QString name() const;
};

#endif // QXRDPILATUSPLUGIN_H

/******************************************************************
*
*  $Log: qxrdpilatusplugin.h,v $
*  Revision 1.2  2010/09/13 20:00:20  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
