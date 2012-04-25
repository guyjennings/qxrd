#include "qxrddebug.h"
#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"
#include "qxrdexperiment.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(QxrdSettingsSaverWPtr saver,
                                                 QxrdExperimentWPtr doc,
                                                 QxrdAcquisitionWPtr acq,
                                                 QxrdAllocatorWPtr allocator,
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
  QxrdDataProcessorPtr p(new QxrdDataProcessor(m_Saver,
                                               m_Experiment,
                                               m_Acquisition,
                                               m_Allocator,
                                               m_FileSaver));

  int rc = -1;

  if (p) {
    p -> initialize();

    m_DataProcessor = p;

    rc = exec();
  }

  {
    QxrdExperimentPtr exp(m_Experiment);

    if (exp && qcepDebug(DEBUG_THREADS)) {
      exp->printMessage(tr("Processor Thread Terminated with rc %1").arg(rc));
    }
  }
}

void QxrdDataProcessorThread::shutdown()
{
  exit();

  wait();
}

QxrdDataProcessorPtr QxrdDataProcessorThread::dataProcessor() const
{
  while (isRunning() && m_DataProcessor == NULL) {
    QThread::msleep(50);
  }

  return m_DataProcessor;
}

void QxrdDataProcessorThread::msleep(unsigned long t)
{
  QThread::msleep(t);
}
