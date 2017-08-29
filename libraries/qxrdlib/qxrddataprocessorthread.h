#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qxrdthread.h"
#include "qxrdexperiment-ptr.h"
#include "qcepallocator-ptr.h"
#include "qxrdfilesaver-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include <QSettings>

class QxrdDataProcessorThread : public QxrdThread
{
  Q_OBJECT

public:
  QxrdDataProcessorThread(QcepObjectWPtr parent);

  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessorPtr dataProcessor() const;

protected:
  void run();

private:
  QxrdDataProcessorPtr    m_DataProcessor;
};

typedef QSharedPointer<QxrdDataProcessorThread> QxrdDataProcessorThreadPtr;

#endif // QXRDDATAPROCESSORTHREAD_H