#ifndef QXRDDETECTORDRIVERTHREAD_H
#define QXRDDETECTORDRIVERTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetectordriver-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrddetectordriverthread-ptr.h"

class QXRD_EXPORT QxrdDetectorDriverThread : public QxrdThread
{
  Q_OBJECT
public:
  explicit QxrdDetectorDriverThread(QxrdDetectorSettingsWPtr det);
  virtual ~QxrdDetectorDriverThread();

  static QxrdDetectorDriverThreadPtr newDetectorDriverThread(QxrdDetectorSettingsWPtr det);

  void setDetectorDriver(QxrdDetectorDriverPtr detDriver);
  QxrdDetectorDriverWPtr detectorDriver() const;

  void shutdown();

signals:

public slots:
  void startDetectorDriver();
  void stopDetectorDriver();

  void changeExposureTime(double expos);
  void beginAcquisition(double expos);
  void endAcquisition();
  void shutdownAcquisition();

  void beginFrame();

protected:
  void run();

private:
  QxrdDetectorSettingsWPtr  m_Detector;
  QxrdDetectorDriverPtr     m_DetectorDriver;
};

#endif // QXRDDETECTORDRIVERTHREAD_H
