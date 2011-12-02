#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  QxrdDataProcessor(QxrdExperiment *doc,
                    QxrdAcquisition *acq,
                    QxrdAllocator *allocator,
                    QxrdFileSaverThread *saver,
                    QObject *parent=0);
  ~QxrdDataProcessor();
};

typedef QSharedPointer<QxrdDataProcessor> QxrdDataProcessorPtr;

#endif
