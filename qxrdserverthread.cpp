/******************************************************************
*
*  $Id: qxrdserverthread.cpp,v 1.9 2009/06/27 22:50:33 jennings Exp $
*
*******************************************************************/

#include "qxrdserverthread.h"

#include "qxrdserver.h"
#include <QMetaObject>

QxrdServerThread::QxrdServerThread(QxrdAcquisitionThread *acq, QString name)
  : m_AcquisitionThread(acq),
    m_Name(name),
    m_Server(NULL),
    SOURCE_IDENT("$Id: qxrdserverthread.cpp,v 1.9 2009/06/27 22:50:33 jennings Exp $")
{
}

QxrdServerThread::~QxrdServerThread()
{
  shutdown();

  delete m_Server;
}

void QxrdServerThread::shutdown()
{
  QMetaObject::invokeMethod(m_Server, "shutdown", Qt::QueuedConnection);

  exit();

  wait(1000);
}

void QxrdServerThread::run()
{
  m_Server = new QxrdServer(m_AcquisitionThread, m_Name);

  connect(m_Server, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));
  connect(this, SIGNAL(execute(QString)), m_Server, SLOT(executeCommand(QString)));

  exec();
}

void QxrdServerThread::executeScript(QString cmd)
{
  emit execute(cmd);
}

/******************************************************************
*
*  $Log: qxrdserverthread.cpp,v $
*  Revision 1.9  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

