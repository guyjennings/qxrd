#ifndef QXRDPROCESSORSTEP_H
#define QXRDPROCESSORSTEP_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qxrdprocessor-ptr.h"

class QXRD_EXPORT QxrdProcessorStep : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdProcessorStep(QString name);
  virtual ~QxrdProcessorStep();

  void initialize(QxrdProcessorWPtr proc);

protected:
  QxrdProcessorWPtr m_Processor;
};

Q_DECLARE_METATYPE(QxrdProcessorStep*)

#endif // QXRDPROCESSORSTEP_H
