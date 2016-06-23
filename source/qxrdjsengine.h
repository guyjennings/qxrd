#ifndef QXRDJSENGINE_H
#define QXRDJSENGINE_H

#include <QtQml>
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"

class QxrdJSEngine : public QQmlEngine
{
public:
  QxrdJSEngine(QxrdApplicationWPtr app, QxrdExperimentWPtr exp);
  void initialize();
  void setWindow(QxrdWindowWPtr win);

private:
  mutable QMutex         m_Mutex;
  QxrdApplicationWPtr    m_Application;
  QxrdExperimentWPtr     m_Experiment;
  QxrdAcquisitionWPtr    m_Acquisition;
  QxrdDataProcessorWPtr  m_DataProcessor;
  QxrdWindowWPtr         m_Window;
};

#endif // QXRDJSENGINE_H
