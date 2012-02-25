#ifndef QXRDACQUIREDIALOGBASE_H
#define QXRDACQUIREDIALOGBASE_H

#include <QDockWidget>
#include <QMenu>

class QxrdExperiment;
class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;

typedef QWeakPointer<QxrdExperiment> QxrdExperimentWPtr;
typedef QWeakPointer<QxrdAcquisition> QxrdAcquisitionWPtr;
typedef QWeakPointer<QxrdDataProcessor> QxrdDataProcessorWPtr;

class QxrdAcquireDialogBase : public QDockWidget
{
    Q_OBJECT
public:
    explicit QxrdAcquireDialogBase(QxrdExperimentWPtr doc,
                                   QxrdWindow *win,
                                   QxrdAcquisitionWPtr acq,
                                   QxrdDataProcessorWPtr proc,
                                   QWidget *parent = 0);

public:
  virtual void setupAcquireMenu(QMenu *menu) = 0;
  virtual void onAcquisitionInit() = 0;
  virtual void acquisitionReady() = 0;

protected:
  QxrdExperimentWPtr    m_Experiment;
  QxrdWindow           *m_Window;
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdDataProcessorWPtr m_DataProcessor;
};

#endif // QXRDACQUIREDIALOGBASE_H
