#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  QxrdDataProcessor(QxrdExperimentWPtr doc,
                    QxrdAcquisitionWPtr acq,
                    QxrdFileSaverWPtr filesaver);
  ~QxrdDataProcessor();

  void initialize();

private:
};

#endif
