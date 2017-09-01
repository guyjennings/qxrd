#include "qxrdapplicationcommand.h"

QxrdApplicationCommand::QxrdApplicationCommand(QString name, QcepApplicationPtr app)
  : QxrdCommand(name),
    m_Application(app)
{

}
