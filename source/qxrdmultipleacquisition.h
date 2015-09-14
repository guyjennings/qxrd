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
  void acquire();

  void onExposureTimeChanged();

  void appendDetector(int detType);
  void appendDetectorProxy(QxrdDetectorProxyPtr proxy);
  void clearDetectors();

  void configureDetector(int i);

  QxrdDetectorThreadPtr detectorThread(int n);
  QxrdDetectorPtr       detector(int n);

public:
  void setupExposureMenu(QDoubleSpinBox *cb);

  void beginAcquisition();
  void endAcquisition();
  void shutdownAcquisition();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

public:
  Q_PROPERTY(int detectorCount READ get_DetectorCount WRITE set_DetectorCount)
  QCEP_INTEGER_PROPERTY(DetectorCount)

private:
  QMutex                         m_Mutex;
  QVector<QxrdDetectorThreadPtr> m_DetectorThreads;
  QVector<QxrdDetectorPtr>       m_Detectors;
};

#endif // QXRDMULTIPLEACQUISITION_H
