#include "qxrddebug.h"
#include "qxrdtestthread.h"

#include "qxrdtestimageplotdatavishelper.h"
#include "qxrdtestimageplotqwthelper.h"
#include "qxrdtestscanplotdatavishelper.h"
#include "qxrdtestscanplotqwthelper.h"

template <class T>
QxrdTestThread<T>::QxrdTestThread(QcepObjectWPtr parent)
  : QxrdThread(parent)
{

}

template <class T>
QxrdTestThread<T>::~QxrdTestThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdTestThread::~QxrdTestThread(%p)\n", this);
  }
}

template <class T>
void QxrdTestThread<T>::shutdown()
{
  exit();

  wait();
}

template <class T>
void QxrdTestThread<T>::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Test Thread Started\n");
  }

  m_Object = QSharedPointer<T>(new T());

  int rc = exec();

  m_Object = QSharedPointer<T>();

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Test Scan Generator Thread Terminated with rc %d\n", rc);
  }
}

template <class T>
QSharedPointer<T> QxrdTestThread<T>::object()
{
  while (isRunning()) {
    if (m_Object) return m_Object;

    QThread::msleep(50);
  }

  return QSharedPointer<T>();
}

template class QxrdTestThread<QxrdTestImagePlotQwtHelper>;
template class QxrdTestThread<QxrdTestScanPlotQwtHelper>;

#ifdef HAVE_DATAVIS
template class QxrdTestThread<QxrdTestImagePlotDataVisHelper>;
template class QxrdTestThread<QxrdTestScanPlotDataVisHelper>;
#endif // HAVE_DATAVIS
