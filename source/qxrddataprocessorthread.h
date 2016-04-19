#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qxrdthread.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include <QSettings>

class QxrdDataProcessorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdDataProcessorThread(QcepSettingsSaverWPtr saver,
                          QxrdExperimentWPtr doc,
                          QxrdAcquisitionWPtr acq,
                          QxrdFileSaverWPtr filesaver);

  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessorPtr dataProcessor() const;

protected:
  void run();

private:
  QxrdFileSaverWPtr       m_FileSaver;
  QxrdAcquisitionWPtr     m_Acquisition;
  QxrdExperimentWPtr      m_Experiment;
  QcepSettingsSaverWPtr   m_Saver;
  QxrdDataProcessorPtr    m_DataProcessor;
};

typedef QSharedPointer<QxrdDataProcessorThread> QxrdDataProcessorThreadPtr;

#endif // QXRDDATAPROCESSORTHREAD_H
