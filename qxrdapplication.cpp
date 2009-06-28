/******************************************************************
*
*  $Id: qxrdapplication.cpp,v 1.39 2009/06/28 11:21:58 jennings Exp $
*
*******************************************************************/

#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdserverthread.h"
#include "qxrdserver.h"
#include "qxrdacquisitionthread.h"
#include "qxrdacquisition.h"
#include "qxrdscriptenginethread.h"
#include "qxrdscriptengine.h"

#include <QTime>
#include <QtConcurrentRun>
#include <QFileDialog>
#include <QMessageBox>

static QxrdApplication* g_Application = NULL;

QxrdApplication::QxrdApplication(int &argc, char **argv)
  : QApplication(argc, argv),
    m_Window(NULL),
    m_ServerThread(NULL),
    m_AcquisitionThread(NULL),
    SOURCE_IDENT("$Id: qxrdapplication.cpp,v 1.39 2009/06/28 11:21:58 jennings Exp $")
{
  setObjectName("qxrdapplication");

//  printf("application thread %p\n", thread());

  g_Application = this;

  setOrganizationName("bessrc");
  setOrganizationDomain("xor.aps.anl.gov");
  setApplicationName("qxrd");

  m_AcquisitionThread = new QxrdAcquisitionThread(/*this, NULL*/);
  m_Acquisition = m_AcquisitionThread -> acquisition();

  m_Window = new QxrdWindow(this, m_AcquisitionThread);
  m_Window -> show();

  connect(this, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));
  emit printMessage("window shown");

  m_AcquisitionThread -> start();

  m_ServerThread = new QxrdServerThread(m_AcquisitionThread, "qxrd");
  m_Server = m_ServerThread -> server();

  emit printMessage("server thread created");

  connect(m_ServerThread,       SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));
  connect(m_Server,             SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));

  m_ServerThread -> start();

  emit printMessage("server thread started");

  m_ScriptEngineThread = new QxrdScriptEngineThread(this, m_Window, m_Acquisition);
  m_ScriptEngine = m_ScriptEngineThread -> scriptEngine();

  emit printMessage("script thread created");

  connect(m_ScriptEngineThread, SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));
  connect(m_ScriptEngine,       SIGNAL(printMessage(QString)), m_Window,            SLOT(printMessage(QString)));

  connect(m_Server,         SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateSpecCommand(QString)));
  connect(m_ScriptEngine,   SIGNAL(specResultAvailable(QScriptValue)), m_Server,          SLOT(finishedCommand(QScriptValue)));

  connect(m_Window,         SIGNAL(executeCommand(QString)),           m_ScriptEngine,    SLOT(evaluateAppCommand(QString)));
  connect(m_ScriptEngine,   SIGNAL(appResultAvailable(QScriptValue)),  m_Window,          SLOT(finishedCommand(QScriptValue)));

  m_Window -> setScriptEngine(m_ScriptEngine);

  m_ScriptEngineThread -> start();

  emit printMessage("script thread started");

//  m_AcquisitionThread -> setWindow(m_Window);

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));
}

QxrdApplication::~QxrdApplication()
{
  delete m_AcquisitionThread;
  delete m_ServerThread;
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

void QxrdApplication::shutdownThreads()
{
  m_Window -> writeSettings();
  m_AcquisitionThread -> shutdown();
  m_ServerThread -> shutdown();
}

QxrdWindow* QxrdApplication::window()
{
  return m_Window;
}

QxrdAcquisitionThread *QxrdApplication::acquisitionThread()
{
  return m_AcquisitionThread;
}

/******************************************************************
*
*  $Log: qxrdapplication.cpp,v $
*  Revision 1.39  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.38  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.37  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

