#ifndef QSWEXPERIMENTTHREAD_H
#define QSWEXPERIMENTTHREAD_H

#include "qswlib_global.h"
#include "qcepthread.h"
#include "qswexperiment-ptr.h"

class /*QSW_EXPORT*/ QswExperimentThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  Q_INVOKABLE QswExperimentThread(QString name);
  ~QswExperimentThread();

  void initialize(QcepObjectWPtr parent);

  QswExperimentWPtr experiment();

protected:
  void run();

private:
  QswExperimentPtr m_Experiment;
};

#endif // QSWEXPERIMENTTHREAD_H
