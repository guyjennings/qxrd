#ifndef QXRDTESTSCANGENERATORTHREAD_H
#define QXRDTESTSCANGENERATORTHREAD_H

#include "qcepmacros.h"
#include "qxrdthread.h"
#include "qxrdtestscangenerator-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdTestScanGeneratorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdTestScanGeneratorThread(QxrdExperimentWPtr doc);
  ~QxrdTestScanGeneratorThread();

  void shutdown();

  QxrdTestScanGeneratorPtr generator() const;

protected:
  void run();

private:
  QxrdTestScanGeneratorPtr m_Generator;
};

#endif // QXRDTESTSCANGENERATORTHREAD_H