#ifndef QXRDDETECTORDRIVERTHREAD_H
#define QXRDDETECTORDRIVERTHREAD_H

#include "qxrdthread.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetectordriver-ptr.h"
#include "qxrddetectorsettings-ptr.h"

class QxrdDetectorDriverThread : public QxrdThread
{
  Q_OBJECT
public:
  explicit QxrdDetectorDriverThread(QxrdDetectorSettingsWPtr det);
  virtual ~QxrdDetectorDriverThread();

  void setDetectorDriver(QxrdDetectorDriverPtr detDriver);
  QxrdDetectorDriverWPtr detectorDriver() const;

  void shutdown();

signals:

public slots:

protected:
  void run();

private:
  QxrdDetectorSettingsWPtr  m_Detector;
  QxrdDetectorDriverPtr     m_DetectorDriver;
};

#endif // QXRDDETECTORDRIVERTHREAD_H
