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

private:
  typedef QxrdThread inherited;

public:
  explicit QxrdDetectorDriverThread(QString name);
  virtual ~QxrdDetectorDriverThread();

  void initialize(QcepObjectWPtr parent);

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

Q_DECLARE_METATYPE(QxrdDetectorDriverThread*)

#endif // QXRDDETECTORDRIVERTHREAD_H
