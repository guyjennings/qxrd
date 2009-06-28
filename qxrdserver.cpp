/******************************************************************
*
*  $Id: qxrdserver.cpp,v 1.11 2009/06/28 04:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"

QxrdServer::QxrdServer(QxrdAcquisitionThread *acqth, QString name, QObject *parent)
  : QSpecServer(name, parent),
    m_AcquisitionThread(acqth),
    SOURCE_IDENT("$Id: qxrdserver.cpp,v 1.11 2009/06/28 04:00:39 jennings Exp $")
{
  emit printMessage("QxrdServer::QxrdServer");
}

QVariant QxrdServer::executeCommand(QString cmd)
{
//  printf("QxrdServer::executeCommand(%s)\n", qPrintable(cmd));

  QVariant result = m_AcquisitionThread->evaluate(cmd);

  return result;
}

QVariant QxrdServer::readProperty(QString name)
{
  QVariant res;

  if (name=="test") {
    return 42;
  }

  return QSpecServer::readProperty(name);
}

void QxrdServer::shutdown()
{
  close();
}

/******************************************************************
*
*  $Log: qxrdserver.cpp,v $
*  Revision 1.11  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.10  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

