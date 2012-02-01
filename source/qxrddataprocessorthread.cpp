#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"
#include "qxrdapplication.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(QxrdSettingsSaver *saver,
                                                 QxrdExperiment *doc,
                                                 QxrdAcquisitionPtr acq,
                                                 QxrdAllocator *allocator,
                                                 QxrdFileSaverThread *filesaver,
                                                 QSettings *settings,
                                                 QString section)
  : QxrdThread(),
    m_Allocator(allocator),
    m_FileSaverThread(filesaver),
    m_DataProcessor(NULL),
    m_Acquisition(acq),
    m_Experiment(doc),
    m_Saver(saver),
    m_Settings(settings),
    m_Section(section)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessorThread::QxrdDataProcessorThread\n");
  }
}

QxrdDataProcessorThread::~QxrdDataProcessorThread()
{
  shutdown();

  delete m_DataProcessor;

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDataProcessorThread::~QxrdDataProcessorThread\n");
  }
}

void QxrdDataProcessorThread::run()
{
  if (qcepDebug(DEBUG_THREADS)) {
    m_Experiment->printMessage("Starting Processor Thread");
  }

  QxrdDataProcessor *p;

  p = new QxrdDataProcessor(m_Saver,
                            m_Experiment,
                            m_Acquisition,
                            m_Allocator,
                            m_FileSaverThread,
                            m_Settings,
                            m_Section,
                            NULL);

  m_DataProcessor.fetchAndStoreOrdered(p);

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
