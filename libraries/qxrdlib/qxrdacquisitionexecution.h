#ifndef QXRDACQUISITIONEXECUTION_H
#define QXRDACQUISITIONEXECUTION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdacqcommon-ptr.h"
#include "qtimer-ptr.h"
#include "qxrdacquisitionparameterpack-ptr.h"
#include "qxrddarkacquisitionparameterpack-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExecution : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdAcquisitionExecution(QString name);
  virtual ~QxrdAcquisitionExecution();
  void initialize(QcepObjectWPtr parent);
  void setAcquisition(QxrdAcqCommonWPtr acq);

signals:

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();
  void doAcquireIdle();

private:
  void executeAcquisition    (QxrdAcquisitionParameterPackPtr     parmsp);
  void executeDarkAcquisition(QxrdDarkAcquisitionParameterPackPtr parmsp);

  void stopIdling();
  void startIdling();

  void accumulateAcquiredImage(QcepUInt16ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepUInt32ImageDataPtr image, QcepUInt32ImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepUInt16ImageDataPtr image, QcepDoubleImageDataPtr accum, QcepMaskDataPtr overflow);
  void accumulateAcquiredImage(QcepUInt32ImageDataPtr image, QcepDoubleImageDataPtr accum, QcepMaskDataPtr overflow);

public:
  Q_PROPERTY(int acquiring READ get_Acquiring WRITE set_Acquiring STORED false)
  QCEP_INTEGER_PROPERTY(Acquiring)

private:
  QxrdAcqCommonWPtr m_Acquisition;
  QTimerPtr         m_IdleTimer;
};

Q_DECLARE_METATYPE(QxrdAcquisitionExecution*)

#endif // QXRDACQUISITIONEXECUTION_H
