#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDataProcessor(QxrdExperimentWPtr doc,
                    QxrdAcquisitionWPtr acq,
                    QxrdFileSaverWPtr filesaver);
  ~QxrdDataProcessor();

  void initialize();

private:
};

Q_DECLARE_METATYPE(QxrdDataProcessor*)

#endif
