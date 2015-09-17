#ifndef QXRDMULTIPLEACQUISITION_H
#define QXRDMULTIPLEACQUISITION_H

#include "qxrdacquisition.h"
#include "qxrddetector.h"
#include "qxrddetector-ptr.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorthread-ptr.h"
#include "qxrddetectorproxy-ptr.h"

class QxrdMultipleAcquisition : public QxrdAcquisition
{
  Q_OBJECT

public:
  QxrdMultipleAcquisition(QcepSettingsSaverWPtr saver,
                          QxrdExperimentWPtr doc,
                          QxrdDataProcessorWPtr proc,
                          QcepAllocatorWPtr allocator);
  virtual ~QxrdMultipleAcquisition();

signals:

public slots:
//  void acquire();

//  void onExposureTimeChanged();

public:
//  void setupExposureMenu(QDoubleSpinBox *cb);

//  void beginAcquisition();
//  void endAcquisition();
//  void shutdownAcquisition();

public:
private:
  QMutex                         m_Mutex;
};

#endif // QXRDMULTIPLEACQUISITION_H
