#ifndef QXRDACQUIREDIALOGBASE_H
#define QXRDACQUIREDIALOGBASE_H

#include <QDockWidget>

class QxrdExperiment;
class QxrdWindow;
class QxrdAcquisition;
class QxrdDataProcessor;
#include <QMenu>

class QxrdAcquireDialogBase : public QDockWidget
{
    Q_OBJECT
public:
    explicit QxrdAcquireDialogBase(QxrdExperiment *doc,
                                   QxrdWindow *win,
                                   QxrdAcquisition *acq,
                                   QxrdDataProcessor *proc,
                                   QWidget *parent = 0);

public:
  virtual void setupAcquireMenu(QMenu *menu) = 0;
  virtual void onAcquisitionInit() = 0;
  virtual void acquisitionReady() = 0;

protected:
  QxrdExperiment    *m_Experiment;
  QxrdWindow        *m_Window;
  QxrdAcquisition   *m_Acquisition;
  QxrdDataProcessor *m_DataProcessor;
};

#endif // QXRDACQUIREDIALOGBASE_H
