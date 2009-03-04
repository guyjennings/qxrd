#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdserverthread.h"
#include "qxrdacquisitionthread.h"

#include <QScriptEngine>
#include <QTime>
#include <QtConcurrentRun>
#include <QFileDialog>

static QxrdApplication* g_Application = NULL;

QxrdApplication::QxrdApplication(int &argc, char **argv)
  : QApplication(argc, argv),
    m_Window(NULL),
    m_ServerThread(NULL),
    m_AcquisitionThread(NULL),
    m_Acquiring(false)
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

  connect(m_AcquisitionThread, SIGNAL(newDataAvailable()),
	  this, SLOT(newDataAvailable()));

  connect(m_AcquisitionThread, SIGNAL(acquireComplete()),
	  this, SLOT(acquireComplete()));

  connect(m_AcquisitionThread, SIGNAL(acquiredFrame(int,int,int,int)),
          m_Window, SLOT(acquiredFrame(int,int,int,int)));

  connect(m_AcquisitionThread, SIGNAL(printMessage(QString)),
	  this, SLOT(printMessage(QString)));

  connect(m_Window->m_ActionAcquire, SIGNAL(triggered()), 
	  this, SLOT(doAcquire()));

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
  QVector<double> integ = m_AcquisitionThread -> integrationTimes();

  int n = integ.count();

  for (int i=0; i<n; i++) {
    m_Window -> setIntegrationTime(i, integ[i]);
  }
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
  m_Window -> printMessage(msg);
}

void QxrdApplication::newDataAvailable()
{
  printMessage("QxrdApplication::newDataAvailable()\n");
}

void QxrdApplication::doAcquire()
{
  printMessage("QxrdApplication::doAcquire()\n");

  m_Window -> setCancelButton();

  int    integmode = m_Window -> integrationMode();
  int    nsum      = m_Window -> nSummed();
  int    nframes   = m_Window -> nFrames();

  m_AcquisitionThread -> acquire(integmode, nsum, nframes);

  m_Acquiring = true;
}

void QxrdApplication::doCancel()
{
  if (m_Acquiring) {
    m_AcquisitionThread -> cancel();
  }
}

void QxrdApplication::acquireComplete()
{
  m_Window -> setAcquireButton();

  m_Acquiring = false;
}

void QxrdApplication::saveData()
{
  QString theFile = QFileDialog::getSaveFileName(m_Window, "Save Data in");

  if (theFile.length()) {
    m_AcquisitionThread -> saveData(theFile);
  }
}
