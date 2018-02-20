#include "qcepapplication.h"
#include "qcepobjecttreewindow.h"
#include <QThread>
#include <QApplication>

QcepApplication *g_Application = NULL;

QcepApplication::QcepApplication(int &argc, char **argv) :
  inherited("application")
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
