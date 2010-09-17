/******************************************************************
*
*  $Id: qxrdserverthread.cpp,v 1.3 2010/09/17 23:12:18 jennings Exp $
*
*******************************************************************/

#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>

QxrdServerThread::QxrdServerThread(QxrdAcquisitionThreadPtr acq, QString name, int port)
  : m_AcquisitionThread(acq),
    m_Name(name),
    m_Port(port),
    m_Server(NULL),
    SOURCE_IDENT("$Id: qxrdserverthread.cpp,v 1.3 2010/09/17 23:12:18 jennings Exp $")
{
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  delete m_Server;
}

QxrdServerPtr QxrdServerThread::server() const
{
  while (m_Server == NULL) {
    QThread::msleep(500);
  }

  return m_Server;
}

void QxrdServerThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection));

  exit();

  wait();
}

void QxrdServerThread::run()
{
//  printf("start server\n");
  m_Server = QxrdServerPtr(new QxrdServer(QxrdAcquisitionThreadPtr(NULL), m_Name, m_Port));

  connect(m_Server,             SIGNAL(printMessage(QString)), this,            SIGNAL(printMessage(QString)));
  connect(m_Server,             SIGNAL(statusMessage(QString)), this,            SIGNAL(statusMessage(QString)));
  connect(m_Server,             SIGNAL(criticalMessage(QString)), this,            SIGNAL(criticalMessage(QString)));

  m_Server -> startServer(QHostAddress::Any, m_Port);

  int rc = exec();

  emit printMessage(tr("spec server started on port %1").arg(m_Server->serverPort()));

//  printf("Server thread terminated with rc %d\n", rc);
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}

/******************************************************************
*
*  $Log: qxrdserverthread.cpp,v $
*  Revision 1.3  2010/09/17 23:12:18  jennings
*  Display port numbers when servers start up
*  Rearrange help files
*
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.4  2010/08/17 19:20:51  jennings
*  Added INVOKE_CHECK macro to check returned result QMetaObject::invokeMethod calls
*
*  Revision 1.1.2.3  2010/08/05 19:29:42  jennings
*  Removed some gratuitous output when starting/stopping socket servers
*
*  Revision 1.1.2.2  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.11.4.8  2010/06/09 19:44:21  jennings
*  Print out termination messages when threads quit
*
*  Revision 1.11.4.7  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.11.4.6  2010/05/02 08:12:07  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.11.4.5  2010/04/27 19:37:51  jennings
*  Explicitly delete some data no longer protected by QSharedPointer s
*
*  Revision 1.11.4.4  2010/04/27 01:31:26  jennings
*  Starting to get QSharedPointer working
*
*  Revision 1.11.4.3  2010/04/26 02:43:31  jennings
*  Called msleep(500) rather than sleep(0.5)
*
*  Revision 1.11.4.2  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.11.4.1  2010/04/21 20:50:41  jennings
*  Modified thread initialization so that objects are created from run method to get correct thread affinity.
*
*  Revision 1.11  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
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

