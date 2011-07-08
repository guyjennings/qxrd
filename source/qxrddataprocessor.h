#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  QxrdDataProcessor(QxrdAcquisition *acq, QxrdAllocator *allocator, QxrdFileSaverThread *saver, QObject *parent=0);
  ~QxrdDataProcessor();

};

#endif
