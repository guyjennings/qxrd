#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdserver.h"
#include "qxrdacquisitionthread.h"

#include <QScriptEngine>
#include <QTime>
#include <QtConcurrentRun>
#include <QFileDialog>
#include <QMessageBox>

static QxrdApplication* g_Application = NULL;

QxrdApplication::QxrdApplication(int &argc, char **argv)
  : QApplication(argc, argv),
    m_Window(NULL),
    m_Server(NULL),
    m_AcquisitionThread(NULL)
{
  setObjectName("qxrdapplication");

  printf("application thread %p\n", thread());

  g_Application = this;

  setOrganizationName("bessrc");
  setOrganizationDomain("xor.aps.anl.gov");
  setApplicationName("qxrd");

  m_AcquisitionThread = new QxrdAcquisitionThread(this, NULL);
  connect(m_AcquisitionThread, SIGNAL(acquisitionRunning()), this, SLOT(acquisitionRunning()));
  connect(m_AcquisitionThread, SIGNAL(printMessage(QString)), this, SLOT(printMessage(QString)));

  m_AcquisitionThread -> start();

  m_Server = new QxrdServer(this, m_AcquisitionThread, "qxrd", NULL);
  connect(m_Server, SIGNAL(print_message(QString)), this, SIGNAL(printMessage(QString)));

  m_Window = new QxrdWindow(this, m_AcquisitionThread);
  m_Window -> show();

  m_AcquisitionThread -> setWindow(m_Window);

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));

  m_ScriptEngine.globalObject().setProperty("acquire",
          m_ScriptEngine.newFunction(QxrdApplication::acquireFunc));

  m_ScriptEngine.globalObject().setProperty("status",
          m_ScriptEngine.newFunction(QxrdApplication::statusFunc));

  m_ScriptEngine.globalObject().setProperty("exposure",
          m_ScriptEngine.newFunction(QxrdApplication::exposureFunc));

  m_ScriptEngine.globalObject().setProperty("subframes",
          m_ScriptEngine.newFunction(QxrdApplication::subframesFunc));

  m_ScriptEngine.globalObject().setProperty("frames",
          m_ScriptEngine.newFunction(QxrdApplication::framesFunc));

  m_ScriptEngine.globalObject().setProperty("filename",
          m_ScriptEngine.newFunction(QxrdApplication::filenameFunc));

  m_ScriptEngine.globalObject().setProperty("directory",
          m_ScriptEngine.newFunction(QxrdApplication::directoryFunc));

  m_ScriptEngine.globalObject().setProperty("fileindex",
          m_ScriptEngine.newFunction(QxrdApplication::fileIndexFunc));
}

QxrdApplication::~QxrdApplication()
{
  delete m_AcquisitionThread;
  delete m_Server;
}

void QxrdApplication::serverRunning()
{
}

void QxrdApplication::possiblyQuit()
{
  if (wantToQuit()) {
    quit();
  }
}

bool QxrdApplication::wantToQuit()
{
  return QMessageBox::question(m_Window, tr("Really Quit?"),
                               tr("Do you really want to exit the application?"),
                                  QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok;
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
  m_AcquisitionThread -> shutdown();
}

QxrdWindow* QxrdApplication::window()
{
  return m_Window;
}

QxrdAcquisitionThread *QxrdApplication::acquisitionThread()
{
  return m_AcquisitionThread;
}

void QxrdApplication::printMessage(QString msg)
{
  m_Window -> printMessage(msg);
}

int QxrdApplication::acquire()
{
  return m_Window -> acquire();
}

int QxrdApplication::acquisitionStatus(double time)
{
  return m_Window -> acquisitionStatus(time);
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
    return QScriptValue(engine, g_Application -> window() -> acquire());
  } else {
    return QScriptValue(engine, -1);
  }
}

QScriptValue QxrdApplication::statusFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    return QScriptValue(engine, g_Application -> window() -> acquisitionStatus(0));
  } else {
    double time = context->argument(0).toNumber();
    return QScriptValue(engine, g_Application -> window() -> acquisitionStatus(time));
  }
}

QScriptValue QxrdApplication::exposureFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Application -> window() -> setExposureTime(context->argument(0).toNumber());
  }

  return QScriptValue(engine, g_Application -> window() -> exposureTime());
}

QScriptValue QxrdApplication::subframesFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Application -> window() -> setNSummed(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Application -> window() -> nSummed());
}

QScriptValue QxrdApplication::framesFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Application -> window() -> setNFrames(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Application -> window() -> nFrames());
}

QScriptValue QxrdApplication::filenameFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Application -> window() -> setFilePattern(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Application -> window() -> filePattern());
}

QScriptValue QxrdApplication::directoryFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Application -> window() -> setOutputDirectory(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Application -> window() -> outputDirectory());
}

QScriptValue QxrdApplication::fileIndexFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Application -> window() -> setFileIndex(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Application -> window() -> fileIndex());
}

