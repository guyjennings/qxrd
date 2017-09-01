#ifndef QXRDAPPLICATIONCOMMAND_H
#define QXRDAPPLICATIONCOMMAND_H

#include <QObject>
#include "qxrdcommand.h"
#include "qcepapplication-ptr.h"

class QxrdApplicationCommand : public QxrdCommand
{
public:
  QxrdApplicationCommand(QString name, QcepApplicationPtr app);

protected:
  QcepApplicationWPtr m_Application;
};

#endif // QXRDAPPLICATIONCOMMAND_H
