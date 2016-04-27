#include "qxrddebug.h"
#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"
#include "qcepmutexlocker.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(QxrdExperimentWPtr doc,
                                                 QxrdAcquisitionWPtr acq,
                                                 QxrdFileSaverWPtr filesaver)
  : QxrdThread(doc),
    m_FileSaver(filesaver),
    m_Acquisition(acq),
    m_Experiment(doc),
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

  {
    QxrdDataProcessorPtr proc = QxrdDataProcessorPtr(new QxrdDataProcessor(m_Experiment,
                                                                           m_Acquisition,
                                                                           m_FileSaver));

    if (proc) {
      proc->initialize();
    }

    m_DataProcessor = proc;
  }

  int rc = exec();

  m_DataProcessor = QxrdDataProcessorPtr();

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
    if (m_DataProcessor) return m_DataProcessor;

    QThread::msleep(50);
  }

  return QxrdDataProcessorPtr();
}
