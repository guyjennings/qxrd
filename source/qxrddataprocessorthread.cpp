#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(QxrdAcquisition *acq, QxrdAllocator *allocator, QxrdFileSaverThread *saver)
  : QxrdThread(),
    m_Allocator(allocator),
    m_FileSaverThread(saver),
    m_DataProcessor(NULL),
    m_Acquisition(acq)
{
}

QxrdDataProcessorThread::~QxrdDataProcessorThread()
{
  shutdown();

  delete m_DataProcessor;
}

void QxrdDataProcessorThread::run()
{
  QxrdDataProcessor *p;

  p = new QxrdDataProcessor(m_Acquisition, m_Allocator, m_FileSaverThread, NULL);

  m_DataProcessor.fetchAndStoreOrdered(p);

  int rc = exec();

//  printf("Processor thread terminated with rc %d\n", rc);
}

void QxrdDataProcessorThread::shutdown()
{
  INVOKE_CHECK(QMetaObject::invokeMethod(m_DataProcessor,"shutdown",Qt::QueuedConnection));

  wait();
}

QxrdDataProcessor *QxrdDataProcessorThread::dataProcessor() const
{
  while (m_DataProcessor == NULL) {
    QThread::msleep(50);
  }

  return m_DataProcessor;
}

void QxrdDataProcessorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}
