#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  QxrdDataProcessor(QxrdDocument *doc,
                    QxrdAcquisition *acq,
                    QxrdAllocator *allocator,
                    QxrdFileSaverThread *saver,
                    QObject *parent=0);
  ~QxrdDataProcessor();
};

typedef QSharedPointer<QxrdDataProcessor> QxrdDataProcessorPtr;

#endif
