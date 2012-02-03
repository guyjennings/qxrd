#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"
#include "qxrdacquisition.h"

class QxrdExperiment;

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  QxrdDataProcessor(QxrdSettingsSaverPtr saver,
                    QxrdExperimentPtr doc,
                    QxrdAcquisitionPtr acq,
                    QxrdAllocatorPtr allocator,
                    QxrdFileSaverPtr filesaver,
                    QSettings *settings,
                    QString section,
                    QObject *parent=0);
  ~QxrdDataProcessor();
};

typedef QSharedPointer<QxrdDataProcessor> QxrdDataProcessorPtr;

#endif
