/******************************************************************
*
*  $Id: qxrdserverthread.cpp,v 1.11 2009/09/20 21:18:53 jennings Exp $
*
*******************************************************************/

#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>

QxrdServerThread::QxrdServerThread(QxrdAcquisitionThread *acq, QString name)
  : m_AcquisitionThread(acq),
    m_Name(name),
    m_Server(NULL),
    SOURCE_IDENT("$Id: qxrdserverthread.cpp,v 1.11 2009/09/20 21:18:53 jennings Exp $")
{
  m_Server = new QxrdServer(NULL, m_Name);

  m_Server -> moveToThread(this);
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  delete m_Server;
}

QxrdServer *QxrdServerThread::server() const
{
  return m_Server;
}

void QxrdServerThread::shutdown()
{
  QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection);

  exit();

  wait(1000);
}

void QxrdServerThread::run()
{
//  printf("start server\n");

  m_Server -> startServer(QHostAddress::Any);

  exec();
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}

/******************************************************************
*
*  $Log: qxrdserverthread.cpp,v $
*  Revision 1.11  2009/09/20 21:18:53  jennings
*  *** empty log message ***
*
*  Revision 1.10  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.9  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

