#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qxrddataprocessorthreaded.h"

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdDataProcessor(QString name);
  QxrdDataProcessor();
  ~QxrdDataProcessor();

//  void initialize(QxrdExperimentWPtr doc,
//                  QxrdAcquisitionWPtr acq,
//                  QxrdFileSaverWPtr fileSaver);

private:
};

Q_DECLARE_METATYPE(QxrdDataProcessor*)

#endif
