/******************************************************************
*
*  $Id: qxrdcudaprocessorplugin.h,v 1.2 2010/09/13 20:00:13 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCUDAPROCESSORPLUGIN_H
#define QXRDCUDAPROCESSORPLUGIN_H

#include <QObject>
#include "qxrdprocessorinterface.h"

class QxrdCudaProcessorPlugin : public QObject, public QxrdProcessorInterface
{
  Q_OBJECT;
  Q_INTERFACES(QxrdProcessorInterface);

public:
    QxrdCudaProcessorPlugin();

    QString name() const;
};

#endif // QXRDCUDAPROCESSORPLUGIN_H

/******************************************************************
*
*  $Log: qxrdcudaprocessorplugin.h,v $
*  Revision 1.2  2010/09/13 20:00:13  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/07/23 19:20:24  jennings
*  Plugins now working
*
*
*******************************************************************/
