#ifndef QXRDACQDUMMY_H
#define QXRDACQDUMMY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qxrdacqcommon.h"

#include "qxrdacqdummy-ptr.h"

class QXRD_EXPORT QxrdAcqDummy : public QxrdAcqCommon
{
  Q_OBJECT

private:
  typedef QxrdAcqCommon inherited;

public:
  Q_INVOKABLE QxrdAcqDummy(QString name);
  ~QxrdAcqDummy();

  void initialize(QcepObjectWPtr parent);
  void shutdown();

  static void registerMetaTypes();

  int detectorCount() const;
  QxrdDetectorSettingsPtr detector(int i) const;

  QxrdSynchronizerWPtr synchronizer();

  QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const;

  void setupAcquisition();
  void acquire();
  void acquireOnce();
  void acquireDark();
  void cancel();
  void trigger();

  int acquisitionStatus(double time);

  void clearDropped();
  void configureDetector(int i);
  void openDetectorControlWindow(int i);

  QxrdDetectorSettingsPtr newDetector(int detType);
  void appendDetector(QxrdDetectorSettingsPtr det);
  void clearDetectors();
};

Q_DECLARE_METATYPE(QxrdAcqDummy*)

#endif // QXRDACQDUMMY_H
