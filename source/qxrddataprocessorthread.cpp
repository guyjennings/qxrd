#include "qxrddebug.h"
#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qcepmutexlocker.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(QcepSettingsSaverWPtr saver,
                                                 QxrdExperimentWPtr doc,
                                                 QxrdAcquisitionWPtr acq,
                                                 QcepAllocatorWPtr allocator,
                                                 QxrdFileSaverWPtr filesaver)
  : QxrdThread(),
    m_Allocator(allocator),
    m_FileSaver(filesaver),
    m_Acquisition(acq),
    m_Experiment(doc),
    m_Saver(saver),
    m_DataProcessor(NULL)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessorThread::QxrdDataProcessorThread(%p)\n", this);
  }

  setObjectName("processor");
}

QxrdDataProcessorThread::~QxrdDataProcessorThread()
{
  shutdown();

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessorThread::~QxrdDataProcessorThread(%p)\n", this);
  }
}

void QxrdDataProcessorThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    printf("Processor Thread Started\n");
  }

  QxrdDataProcessorPtr p(new QxrdDataProcessor(m_Saver,
                                               m_Experiment,
                                               m_Acquisition,
                                               m_Allocator,
                                               m_FileSaver));

  int rc = -1;

  if (p) {
    p -> initialize();

    m_Mutex.lock();
    m_DataProcessor = p;
    m_Mutex.unlock();

    rc = exec();
  }


  {
    QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    m_DataProcessor = QxrdDataProcessorPtr();
  }

  if (qcepDebug(DEBUG_THREADS)) {
    printf("Processor Thread Terminated with rc %d\n", rc);
  }
}

void QxrdDataProcessorThread::shutdown()
{
  exit();

  wait();
}

QxrdDataProcessorPtr QxrdDataProcessorThread::dataProcessor() const
{
  while (isRunning()) {
    {
      QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

      if (m_DataProcessor) return m_DataProcessor;
    }

    QThread::msleep(50);
  }

  return QxrdDataProcessorPtr();
}
