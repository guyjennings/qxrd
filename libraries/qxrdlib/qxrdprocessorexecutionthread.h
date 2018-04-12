#ifndef QXRDPROCESSOREXECUTIONTHREAD_H
#define QXRDPROCESSOREXECUTIONTHREAD_H

#include "qxrdlib_global.h"
#include "qxrdthread.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdprocessorexecution-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QXRD_EXPORT QxrdProcessorExecutionThread : public QxrdThread
{
  Q_OBJECT

private:
  typedef QxrdThread inherited;

public:
  Q_INVOKABLE QxrdProcessorExecutionThread(QString name);
  ~QxrdProcessorExecutionThread();

  void initialize(QcepObjectWPtr parent);

  void shutdown();

  Q_INVOKABLE void processAcquiredImage(QcepUInt32ImageDataPtr image, QcepMaskDataPtr overflow);
  Q_INVOKABLE void processDarkImage    (QcepDoubleImageDataPtr image, QcepMaskDataPtr overflow);
  Q_INVOKABLE void processIdleImage    (QcepImageDataBasePtr image);

protected:
  void run();

private:
  QxrdProcessorWPtr         m_Processor;
  QxrdProcessorExecutionPtr m_ProcessorExecution;
};

Q_DECLARE_METATYPE(QxrdProcessorExecutionThread*)

#endif // QXRDPROCESSOREXECUTIONTHREAD_H