#include "qxrdsynchronizerthread.h"
#include "qxrddebug.h"
#include "qxrdapplication.h"
#include "qxrdapplication-ptr.h"
#include "qxrdsynchronizerplugin.h"
#include "qxrdsynchronizer.h"

QxrdSynchronizerThread::QxrdSynchronizerThread(QString name) :
  inherited(name),
  m_SynchronizerPlugin(),
  m_Synchronizer()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSynchronizerThread::QxrdSynchronizerThread(%p)\n", this);
  }
}

void QxrdSynchronizerThread::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  QxrdApplicationPtr app(
        qSharedPointerDynamicCast<QxrdApplication>(
          QxrdAppCommon::findApplication(parent)));

  if (app) {
    m_SynchronizerPlugin = app->synchronizerPlugin();
  }
}

QxrdSynchronizerThread::~QxrdSynchronizerThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSynchronizerThread::~QxrdSynchronizerThread(%p)\n", this);
  }
}

void QxrdSynchronizerThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Synchronizer Thread Started\n");
  }

  QxrdSynchronizerPluginPtr syncp(m_SynchronizerPlugin);

  if (syncp) {
    m_Synchronizer = syncp->createSynchronizer("synchronizer");

    if (m_Synchronizer) {
      m_Synchronizer -> initialize(sharedFromThis());
    }
  }

  syncp = QxrdSynchronizerPluginPtr();

  int rc = 0;

  if (m_Synchronizer) {
    rc = exec();
  }

  if (m_Synchronizer) {
    m_Synchronizer -> stopSynchronizer();
    m_Synchronizer = QxrdSynchronizerPtr();
  }

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Synchronizer Thread Terminated with rc %d\n", rc);
  }
}

QxrdSynchronizerWPtr QxrdSynchronizerThread::synchronizer() const
{
  while (isRunning()) {
    if (m_Synchronizer) return m_Synchronizer;

    msleep(50);
  }

  return m_Synchronizer;
}
