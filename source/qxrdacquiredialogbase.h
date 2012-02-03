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
    explicit QxrdAcquireDialogBase(QxrdExperimentPtr doc,
                                   QxrdWindow *win,
                                   QxrdAcquisition *acq,
                                   QxrdDataProcessorPtr proc,
                                   QWidget *parent = 0);

public:
  virtual void setupAcquireMenu(QMenu *menu) = 0;
  virtual void onAcquisitionInit() = 0;
  virtual void acquisitionReady() = 0;

protected:
  QxrdExperimentPtr    m_Experiment;
  QxrdWindow          *m_Window;
  QxrdAcquisitionPtr   m_Acquisition;
  QxrdDataProcessorPtr m_DataProcessor;
};

#endif // QXRDACQUIREDIALOGBASE_H
