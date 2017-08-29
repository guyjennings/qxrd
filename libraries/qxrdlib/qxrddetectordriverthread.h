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
  bool startDetectorDriver();
  bool stopDetectorDriver();

  bool changeExposureTime(double expos);
  bool beginAcquisition(double expos);
  bool endAcquisition();
  bool shutdownAcquisition();

  void beginFrame();

protected:
  void run();

private:
  QxrdDetectorSettingsWPtr  m_Detector;
  QxrdDetectorDriverPtr     m_DetectorDriver;
};

#endif // QXRDDETECTORDRIVERTHREAD_H
