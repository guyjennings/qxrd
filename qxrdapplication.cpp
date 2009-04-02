#include "qxrdapplication.h"
#include "qxrdwindow.h"
#include "qxrdserverthread.h"
#include "qxrdacquisitionthread.h"

#include <QTime>
#include <QtConcurrentRun>
#include <QFileDialog>
#include <QMessageBox>

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

  m_AcquisitionThread = new QxrdAcquisitionThread(/*this, NULL*/);
  connect(m_AcquisitionThread, SIGNAL(acquisitionRunning()), this, SLOT(acquisitionRunning()));
  connect(m_AcquisitionThread, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));

  m_AcquisitionThread -> start();

  m_ServerThread = new QxrdServerThread(m_AcquisitionThread, "qxrd");
  connect(m_ServerThread, SIGNAL(printMessage(QString)), this, SIGNAL(printMessage(QString)));

  m_ServerThread -> start();

  m_Window = new QxrdWindow(this, m_AcquisitionThread);
  m_Window -> show();

  connect(this, SIGNAL(printMessage(QString)), m_Window, SLOT(printMessage(QString)));

//  m_AcquisitionThread -> setWindow(m_Window);

  connect(this, SIGNAL(aboutToQuit()), this, SLOT(shutdownThreads()));
}

QxrdApplication::~QxrdApplication()
{
  delete m_AcquisitionThread;
  delete m_ServerThread;
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

void QxrdApplication::shutdownThreads()
{
  m_Window -> saveSettings();
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

//void QxrdApplication::printMessage(QString msg)
//{
//  m_Window -> printMessage(msg);
//}

//int QxrdApplication::acquire()
//{
//  return m_Window -> acquire();
//}
//
//int QxrdApplication::acquisitionStatus(double time)
//{
//  return m_Window -> acquisitionStatus(time);
//}

