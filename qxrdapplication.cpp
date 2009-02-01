#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdserverthread.h"
#include "qxrdacquisitionthread.h"

#include <QScriptEngine>
#include <QTime>
#include <QtConcurrentRun>

static QxrdApplication* g_Application = NULL;

QxrdApplication::QxrdApplication(int &argc, char **argv)
  : QApplication(argc, argv),
    m_Window(NULL),
    m_ServerThread(NULL),
    m_AcquisitionThread(NULL)
{
  setObjectName("qxrdapplication");

  printf("application thread %p\n", thread());

  g_Application = this;

  setOrganizationName("bessrc");
  setOrganizationDomain("xor.aps.anl.gov");
  setApplicationName("qxrd");

  m_Window = new QxrdWindow(this);
  m_Window -> show();

  m_AcquisitionThread = new QxrdAcquisitionThread(this);
  connect(m_AcquisitionThread, SIGNAL(acquisitionRunning()), this, SLOT(acquisitionRunning()));
  m_AcquisitionThread -> start();

  m_ServerThread = new QxrdServerThread(this, m_AcquisitionThread);
  connect(m_ServerThread, SIGNAL(serverRunning()), this, SLOT(serverRunning()));
  m_ServerThread -> start();

  m_Window -> setAcquisitionThread(m_AcquisitionThread);

  connect(m_ServerThread, SIGNAL(print_message(QString)), this, SLOT(printMessage(QString)));

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

//   connect(m_Window->acquireButton(), SIGNAL(clicked()), 
// 	  this, SLOT(acquire()));

//   connect(m_Window->acquireAction(), SIGNAL(triggered()), 
// 	  this, SLOT(acquire()));

  connect(m_AcquisitionThread, SIGNAL(newDataAvailable()),
	  this, SLOT(newDataAvailable()));

  connect(m_Window->m_ActionPreferences, SIGNAL(triggered()),
	  this, SLOT(doPreferences()));

  connect(m_Window->m_ActionEvaluateScript, SIGNAL(triggered()),
	  this, SLOT(doScript()));
}

QxrdApplication::~QxrdApplication()
{
  delete m_AcquisitionThread;
  delete m_ServerThread;
}

void QxrdApplication::serverRunning()
{
}

void QxrdApplication::acquisitionRunning()
{
}

void QxrdApplication::executeCommand(QString cmd)
{
  QScriptValue result = m_ScriptEngine.evaluate(cmd);

  if (m_ScriptEngine.hasUncaughtException()) {
    emit finishedCommand(tr("Error: ") + m_ScriptEngine.uncaughtException().toString());
  } else {
    emit finishedCommand(result.toString());
  }
}

QScriptValue QxrdApplication::evaluate(QString cmd)
{
  return m_ScriptEngine.evaluate(cmd);
}

void QxrdApplication::shutdownThreads()
{
  m_ServerThread -> shutdown();
  m_AcquisitionThread -> shutdown();
}

QxrdAcquisitionThread *QxrdApplication::acquisitionThread()
{
  return m_AcquisitionThread;
}

QxrdServerThread *QxrdApplication::serverThread()
{
  return m_ServerThread;
}

void QxrdApplication::printMessage(QString msg)
{
  printf("%s\n", qPrintable(msg));
}

void QxrdApplication::newDataAvailable()
{
  printf("QxrdApplication::newDataAvailable()\n");
}

