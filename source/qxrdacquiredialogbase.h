#ifndef QXRDACQUIREDIALOGBASE_H
#define QXRDACQUIREDIALOGBASE_H

#include <QDockWidget>
#include "qxrdacquisition.h"
#include "qxrddataprocessor.h"

class QxrdExperiment;
class QxrdWindow;

#include <QMenu>

class QxrdAcquireDialogBase : public QDockWidget
{
    Q_OBJECT
public:
    explicit QxrdAcquireDialogBase(QxrdExperimentWPtr doc,
                                   QxrdWindowWPtr win,
                                   QxrdAcquisitionWPtr acq,
                                   QxrdDataProcessorWPtr proc,
                                   QWidget *parent = 0);

public:
  virtual void setupAcquireMenu(QMenu *menu) = 0;
  virtual void onAcquisitionInit() = 0;
  virtual void acquisitionReady() = 0;

protected:
  QxrdExperimentWPtr    m_Experiment;
  QxrdWindowWPtr        m_Window;
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdDataProcessorWPtr m_DataProcessor;
};

#endif // QXRDACQUIREDIALOGBASE_H
