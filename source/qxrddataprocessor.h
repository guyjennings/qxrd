#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded, public QEnableSharedFromThis<QxrdDataProcessor>
{
  Q_OBJECT

public:
  QxrdDataProcessor(QcepSettingsSaverWPtr saver,
                    QxrdExperimentWPtr doc,
                    QxrdAcquisitionWPtr acq,
                    QcepAllocatorWPtr allocator,
                    QxrdFileSaverWPtr filesaver);
  ~QxrdDataProcessor();

  void initialize();

private:
};

#endif
