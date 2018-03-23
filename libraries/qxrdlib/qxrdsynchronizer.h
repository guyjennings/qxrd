#ifndef QXRDSYNCHRONIZER_H
#define QXRDSYNCHRONIZER_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"

class QXRD_EXPORT QxrdSynchronizer : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QxrdSynchronizer(QString name);

  virtual void stopSynchronizer() = 0;

  virtual QString name() const = 0;

  virtual int detectorDeviceCount() = 0;
  virtual QString detectorDeviceName(int n) = 0;

  virtual int outputDeviceCount() = 0;
  virtual QString outputDeviceName(int n) = 0;

  virtual int inputDeviceCount() = 0;
  virtual QString inputDeviceName(int n) = 0;

  virtual void updateSyncWaveforms     (QxrdSynchronizedAcquisitionWPtr      s,
                                        QxrdAcquisitionParameterPackWPtr     p) = 0;
  virtual void prepareForAcquisition   (QxrdSynchronizedAcquisitionWPtr      s,
                                        QxrdAcquisitionParameterPackWPtr     p) = 0;
  virtual void prepareForDarkAcquistion(QxrdSynchronizedAcquisitionWPtr      s,
                                        QxrdDarkAcquisitionParameterPackWPtr p) = 0;
};

#endif // QXRDSYNCHRONIZER_H
