#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"
#include "qxrdacquisition.h"

class QxrdExperiment;

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
                    QString section,
                    QObject *parent=0);
  ~QxrdDataProcessor();

  void init();
};

typedef QSharedPointer<QxrdDataProcessor> QxrdDataProcessorPtr;
typedef QWeakPointer<QxrdDataProcessor> QxrdDataProcessorWPtr;

#endif
