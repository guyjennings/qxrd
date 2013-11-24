#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  QxrdDataProcessor(QxrdSettingsSaverWPtr saver,
                    QxrdExperimentWPtr doc,
                    QxrdAcquisitionWPtr acq,
                    QxrdAllocatorWPtr allocator,
                    QxrdFileSaverWPtr filesaver);
  ~QxrdDataProcessor();

  void initialize(QxrdDataProcessorWPtr proc);

private:
  QxrdDataProcessorWPtr m_Processor;
};

#endif
