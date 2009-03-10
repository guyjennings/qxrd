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
  connect(m_AcquisitionThread, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));

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

  connect(m_AcquisitionThread, SIGNAL(acquiredFrame(QString,int,int,int,int,int)),
          m_Window, SLOT(acquiredFrame(QString,int,int,int,int,int)));

  connect(m_AcquisitionThread, SIGNAL(summedFrameCompleted(QString,int)),
	  m_Window, SLOT(summedFrameCompleted(QString,int)));

  connect(m_AcquisitionThread, SIGNAL(fileIndexChanged(int)),
	  m_Window, SLOT(setFileIndex(int)));
  connect(m_AcquisitionThread, SIGNAL(statusMessage(QString)),
          m_Window, SLOT(statusMessage(QString)));


  connect(m_Window->m_ActionAcquire, SIGNAL(triggered()),
          this, SLOT(doAcquire()));

  connect(m_Window->m_ActionCancel, SIGNAL(triggered()),
          this, SLOT(doCancel()));

  connect(m_Window->m_ActionPreferences, SIGNAL(triggered()),
	  this, SLOT(doPreferences()));

  connect(m_Window->m_ActionEvaluateScript, SIGNAL(triggered()),
	  this, SLOT(doScript()));

  m_ScriptEngine.globalObject().setProperty("acquire",
          m_ScriptEngine.newFunction(QxrdApplication::acquireFunc));

  m_ScriptEngine.globalObject().setProperty("status",
          m_ScriptEngine.newFunction(QxrdApplication::statusFunc));
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

  m_Window -> acquisitionReady();
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
  m_Window -> saveSettings();
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

int QxrdApplication::acquire()
{
  doAcquire();

  return 0;
}

int QxrdApplication::acquisitionStatus(double time)
{
  return 0;
}

void QxrdApplication::doAcquire()
{
  printMessage("QxrdApplication::doAcquire()\n");

  m_Window -> acquisitionStarted();

  QString outDir   = m_Window -> outputDirectory();
  QString filePatt = m_Window -> filePattern();
  int    index     = m_Window -> fileIndex();
  int    integmode = m_Window -> integrationMode();
  int    nsum      = m_Window -> nSummed();
  int    nframes   = m_Window -> nFrames();

  m_AcquisitionThread -> acquire(outDir, filePatt, index, integmode, nsum, nframes);

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
  m_Window -> acquisitionFinished();

  m_Acquiring = false;
}

void QxrdApplication::saveData()
{
  QString theFile = QFileDialog::getSaveFileName(m_Window, "Save Data in");

  if (theFile.length()) {
    m_AcquisitionThread -> saveData(theFile);
  }
}

void QxrdApplication::loadData()
{
  QString theFile = QFileDialog::getOpenFileName(m_Window, "Load Image from...");

  if (theFile.length()) {
    m_AcquisitionThread -> loadData(theFile);
  }
}

QScriptValue QxrdApplication::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
    if (context->argumentCount() == 0) {
      return QScriptValue(engine, g_Application -> acquire());
    } else {
      return QScriptValue(engine, -1);
    }
}

QScriptValue QxrdApplication::statusFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    return QScriptValue(engine, g_Application -> acquisitionStatus(0));
  } else {
    double time = context->argument(0).toNumber();
    return QScriptValue(engine, g_Application -> acquisitionStatus(time));
  }
}

