#include "qcepapplication.h"
#include "qcepobjecttreewindow.h"
#include <QThread>
#include <QApplication>

QcepApplication *g_Application = NULL;

QcepApplication::QcepApplication(int &argc, char **argv) :
  inherited("application"),
  m_Argc(this, "argc", argc, "Number of Command Line Arguments"),
  m_Argv(this, "argv", makeStringListFromArgs(argc, argv), "Command Line Arguments"),
  m_GuiWanted(this, "guiWanted", 1, "GUI Wanted?"),
  m_CurrentExperiment(this, "currentExperiment", "", "Current Experiment"),
  m_RecentExperiments(this, "recentExperiments", QStringList(), "Recent Experiments"),
  m_RecentExperimentsSize(this,"recentExperimentsSize", 8, "Number of Recent Experiments to Remember"),
  m_ExperimentCount(this, "experimentCount", 0, "Number of open experiments"),
  m_UpdateIntervalMsec(this, "updateIntervalMsec", 1000, "Time Intervale for Updates (in msec)")
{
  g_Application = this;

  m_Application =
      QApplicationPtr(
        new QApplication(argc, argv));
}

void QcepApplication::initializeRoot()
{
  inherited::initializeRoot();
}

QcepApplicationWPtr QcepApplication::findApplication(QcepObjectWPtr p)
{
  QcepApplicationWPtr res =
      qSharedPointerDynamicCast<QcepApplication>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findApplication(objp->parentPtr());
    }
  }

  return res;
}

int QcepApplication::exec()
{
  int res = 0;

  if (m_Application) {
    res = m_Application -> exec();
  }

  return res;
}

void QcepApplication::processEvents() const
{
  if (m_Application) {
    m_Application -> processEvents();
  }
}

void QcepApplication::exit()
{
  if (m_Application) {
    m_Application -> exit();
  }
}

void QcepApplication::quit()
{
  if (m_Application) {
    m_Application -> quit();
  }
}

void QcepApplication::openObjectBrowserWindow(QcepObjectWPtr obj)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "openObjectBrowserWindow",
                                           Q_ARG(QcepObjectWPtr, obj)));
  } else {
    QcepObjectTreeWindow *w =
        new QcepObjectTreeWindow(NULL, obj);

    w->show();
    w->raise();
  }
}

QStringList QcepApplication::makeStringListFromArgs(int argc, char **argv)
{
  QStringList res;

  for (int i=0; i<argc; i++) {
    res.append(argv[i]);
  }

  return res;
}
