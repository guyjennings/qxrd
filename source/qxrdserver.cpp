/******************************************************************
*
*  $Id: qxrdserver.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"

QxrdServer::QxrdServer(QxrdAcquisitionThreadPtr acqth, QString name, int port, QObject *parent)
  : QSpecServer(name, port, parent),
    m_AcquisitionThread(acqth),
    SOURCE_IDENT("$Id: qxrdserver.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $")
{
  emit printMessage("QxrdServer::QxrdServer");
}

QVariant QxrdServer::executeCommand(QString /*cmd*/)
{
//  printf("QxrdServer::executeCommand(%s)\n", qPrintable(cmd));

//  QVariant result = m_AcquisitionThread->evaluate(cmd);

  return 42;
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
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.12.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.12  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.11  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.10  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

