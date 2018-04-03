#ifndef QXRDPROCESSOREXECUTION_H
#define QXRDPROCESSOREXECUTION_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdprocessor-ptr.h"

class QXRD_EXPORT QxrdProcessorExecution : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdProcessorExecution(QString name);
  virtual ~QxrdProcessorExecution();
  void initialize(QcepObjectWPtr parent);
  static void registerMetaTypes();

public:
  Q_PROPERTY(int processing READ get_Processing WRITE set_Processing STORED false)
  QCEP_INTEGER_PROPERTY(Processing)

private:
  QxrdProcessorWPtr m_Processor;
};

Q_DECLARE_METATYPE(QxrdProcessorExecution*)

#endif // QXRDPROCESSOREXECUTION_H
