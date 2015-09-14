#ifndef QXRDSINGLEACQUISITION_H
#define QXRDSINGLEACQUISITION_H

#include "qxrdacquisition.h"

class QxrdSingleAcquisition : public QxrdAcquisition
{
  Q_OBJECT

public:
  QxrdSingleAcquisition(QcepSettingsSaverWPtr saver,
                        QxrdExperimentWPtr doc,
                        QxrdDataProcessorWPtr proc,
                        QcepAllocatorWPtr allocator);
  virtual ~QxrdSingleAcquisition();

  void setDetector(QxrdDetectorWPtr det);

signals:

public slots:
  void onExposureTimeChanged();

  QxrdDetectorPtr detector();

public:
  void setupExposureMenu(QDoubleSpinBox *cb);

  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

private:
  QxrdDetectorWPtr       m_Detector;
};

#endif // QXRDSINGLEACQUISITION_H
