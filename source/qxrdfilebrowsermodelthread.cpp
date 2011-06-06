#include "qxrdfilebrowsermodelthread.h"
#include "qxrdapplication.h"

QxrdFileBrowserModelThread::QxrdFileBrowserModelThread(QObject *parent) :
    QThread(),
    m_FileBrowserModel(NULL)
{
}

QxrdFileBrowserModel* QxrdFileBrowserModelThread::fileBrowserModel() const
{
  while (m_FileBrowserModel == NULL) {
    QThread::msleep(50);
  }

  return m_FileBrowserModel;
}

void QxrdFileBrowserModelThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage("Starting File Browser Model Thread");
  }

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    g_Application->printMessage(tr("File Browser Model Thread Terminated with rc %1").arg(rc));
  }
}
