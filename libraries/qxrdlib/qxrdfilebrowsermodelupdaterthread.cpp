#include "qxrddebug.h"
#include "qxrdfilebrowsermodelupdaterthread.h"
#include "qxrdfilebrowsermodel.h"
#include "qxrdfilebrowsermodelupdater.h"
#include "qxrdapplication.h"
#include "qcepmutexlocker.h"

QxrdFileBrowserModelUpdaterThread::QxrdFileBrowserModelUpdaterThread(QString name) :
  inherited(name),
  m_Model(),
  m_Updater()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileBrowserModelUpdaterThread::QxrdFileBrowserModelUpdaterThread(%p)\n", this);
  }
}

QxrdFileBrowserModelUpdaterThread::~QxrdFileBrowserModelUpdaterThread()
{
//  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdFileBrowserModelUpdaterThread::~QxrdFileBrowserModelUpdaterThread(%p)\n", this);
  }
}

void QxrdFileBrowserModelUpdaterThread::setModel(QxrdFileBrowserModelWPtr model)
{
  m_Model = model;
}

void QxrdFileBrowserModelUpdaterThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Browser Model Updater Thread Started\n");
  }

  m_Updater = QxrdFileBrowserModelUpdaterPtr(
        NEWPTR(QxrdFileBrowserModelUpdater("browserModelUpdater")));

  m_Updater -> initialize(sharedFromThis());

  m_Updater -> setBrowserModel(m_Model);

  int rc = exec();

  m_Updater = QxrdFileBrowserModelUpdaterPtr();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Browser Model Updater Thread Terminated with rc %d\n", rc);
  }
}

QxrdFileBrowserModelUpdaterPtr QxrdFileBrowserModelUpdaterThread::updater() const
{
  while (isRunning()) {
    if (m_Updater) return m_Updater;

    msleep(50);
  }

  return QxrdFileBrowserModelUpdaterPtr();
}
