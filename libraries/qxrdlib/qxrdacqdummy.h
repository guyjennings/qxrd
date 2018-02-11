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

  static QxrdAcqDummyPtr newAcquisition();

  int detectorCount() const;
  QxrdDetectorSettingsPtr detector(int i) const;
  void setNIDAQPlugin(QxrdNIDAQPluginInterfacePtr nidaqPlugin);
  QxrdNIDAQPluginInterfaceWPtr nidaqPlugin() const;

  QxrdSynchronizedAcquisitionPtr synchronizedAcquisition() const;
  QxrdAcquisitionExtraInputsPtr acquisitionExtraInputs() const;

  void acquire();
  void acquireOnce();
  void acquireDark();
  void cancel();
  void trigger();

  int acquisitionStatus(double time);

  void clearDropped();
  void configureDetector(int i);
  void openDetectorControlWindow(int i);

  void openWindows();
  void closeWindows();

  QxrdDetectorSettingsPtr newDetector(int detType);
  void appendDetector(QxrdDetectorSettingsPtr det);
  void clearDetectors();

  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();
};

Q_DECLARE_METATYPE(QxrdAcqDummy*)

#endif // QXRDACQDUMMY_H
