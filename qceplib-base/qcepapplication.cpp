#include "qcepapplication.h"
#include "qcepobjecttreewindow.h"
#include <QThread>

QcepApplication *g_Application = NULL;

QcepApplication::QcepApplication(int &argc, char **argv) :
  inherited(argc, argv)
{
  g_Application = this;
}

void QcepApplication::initialize()
{
}

void QcepApplication::openObjectBrowser(QcepObjectWPtr obj)
{
  if (QThread::currentThread() != thread()) {
    INVOKE_CHECK(QMetaObject::invokeMethod(this,
                                           "openObjectBrowser",
                                           Q_ARG(QcepObjectWPtr, obj)));
  } else {
    QcepObjectTreeWindow *w =
        new QcepObjectTreeWindow(NULL, obj);

    w->show();
    w->raise();
  }
}
