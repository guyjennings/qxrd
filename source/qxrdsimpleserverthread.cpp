/******************************************************************
*
*  $Id: qxrdsimpleserverthread.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#include "qxrdsimpleserver.h"
#include "qxrdsimpleserverthread.h"

QxrdSimpleServerThread::QxrdSimpleServerThread(QxrdAcquisitionThreadPtr acq, QString name, int port) :
    m_AcquisitionThread(acq),
    m_Name(name),
    m_Port(port),
    m_Server(NULL),
    SOURCE_IDENT("$Id: qxrdsimpleserverthread.cpp,v 1.2 2010/09/13 20:00:42 jennings Exp $")
{
}

QxrdSimpleServerThread::~QxrdSimpleServerThread()
{
  shutdown();

  delete m_Server;
}

QxrdSimpleServerPtr QxrdSimpleServerThread::server() const
{
  while (m_Server == NULL) {
    QThread::msleep(500);
  }

  return m_Server;
}

void QxrdSimpleServerThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection));

  exit();

  wait();
}

void QxrdSimpleServerThread::run()
{
//  printf("start server\n");
  m_Server = QxrdSimpleServerPtr(new QxrdSimpleServer(QxrdAcquisitionThreadPtr(NULL), m_Name, m_Port));

  m_Server -> startServer(QHostAddress::Any, m_Port);

  int rc = exec();

//  printf("Server thread terminated with rc %d\n", rc);
}

/******************************************************************
*
*  $Log: qxrdsimpleserverthread.cpp,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.3  2010/08/17 19:20:51  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.2  2010/08/05 19:29:42  jennings
*  Removed some gratuitous output when starting/stopping socket servers
*
*  Revision 1.1.2.1  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*
*******************************************************************/

