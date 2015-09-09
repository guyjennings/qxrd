#ifndef QXRDMULTIPLEACQUISITION_H
#define QXRDMULTIPLEACQUISITION_H

#include "qxrdacquisition.h"
#include "qxrddetector.h"
#include "qxrddetector-ptr.h"
#include "qxrddetectorthread.h"
#include "qxrddetectorthread-ptr.h"

class QxrdMultipleAcquisition : public QxrdAcquisition
{
  Q_OBJECT

public:
  QxrdMultipleAcquisition(QcepSettingsSaverWPtr saver,
                          QxrdExperimentWPtr doc,
                          QxrdDataProcessorWPtr proc,
                          QcepAllocatorWPtr allocator);

signals:

public slots:
  void acquire();

  void onExposureTimeChanged();
  void onBinningModeChanged();
  void onCameraGainChanged();

  void appendDetector(int detType);
  void clearDetectors();

  QxrdDetectorPtr detector(int n);

public:
  void setupExposureMenu(QDoubleSpinBox *cb);
  void setupCameraGainMenu(QComboBox *cb);
  void setupCameraBinningModeMenu(QComboBox *cb);

  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

public:
  Q_PROPERTY(int detectorCount READ get_DetectorCount WRITE set_DetectorCount)
  QCEP_INTEGER_PROPERTY(DetectorCount)

private:
  QVector<QxrdDetectorThreadPtr> m_DetectorThreads;
  QVector<QxrdDetectorPtr>       m_Detectors;
};

#endif // QXRDMULTIPLEACQUISITION_H
