#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(QxrdSettingsSaverPtr saver,
                                                 QxrdExperimentPtr doc,
                                                 QxrdAcquisitionPtr acq,
                                                 QxrdAllocatorPtr allocator,
                                                 QxrdFileSaverPtr filesaver,
                                                 QSettings *settings,
                                                 QString section)
  : QxrdThread(),
    m_Allocator(allocator),
    m_FileSaver(filesaver),
    m_DataProcessor(NULL),
    m_Acquisition(acq),
    m_Experiment(doc),
    m_Saver(saver),
    m_Settings(settings),
    m_Section(section)
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
  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage("Starting Processor Thread");
  }

  QxrdDataProcessorPtr p;

  p = QxrdDataProcessorPtr(
        new QxrdDataProcessor(m_Saver,
                            m_Experiment,
                            m_Acquisition,
                            m_Allocator,
                            m_FileSaver,
                            m_Settings,
                            m_Section,
                            NULL));

  m_DataProcessor = p;

  int rc = exec();

  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage(tr("Processor Thread Terminated with rc %1").arg(rc));
  }
}

void QxrdDataProcessorThread::shutdown()
{
  exit();

  wait();
}

QxrdDataProcessorPtr QxrdDataProcessorThread::dataProcessor() const
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
