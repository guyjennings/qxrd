#ifndef QXRDTESTIMAGEGENERATORTHREAD_H
#define QXRDTESTIMAGEGENERATORTHREAD_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qxrdthread.h"
#include "qxrdtestimagegenerator-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdTestImageGeneratorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdTestImageGeneratorThread(QxrdExperimentWPtr doc);
  ~QxrdTestImageGeneratorThread();

  void shutdown();

  QxrdTestImageGeneratorPtr generator();

protected:
  void run();

private:
  QxrdTestImageGeneratorPtr m_Generator;
};

#endif // QXRDTESTIMAGEGENERATORTHREAD_H
