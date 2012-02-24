#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"
#include "qxrdacquisition.h"
#include "qxrdexperiment.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  QxrdDataProcessor(QxrdSettingsSaverWPtr saver,
                    QxrdExperimentWPtr doc,
                    QxrdAcquisitionWPtr acq,
                    QxrdAllocatorWPtr allocator,
                    QxrdFileSaverWPtr filesaver,
                    QSettings *settings,
                    QString section);
  ~QxrdDataProcessor();

  void init();
};

#endif

class QxrdDataProcessor;
typedef QSharedPointer<QxrdDataProcessor> QxrdDataProcessorPtr;
typedef QWeakPointer<QxrdDataProcessor> QxrdDataProcessorWPtr;
