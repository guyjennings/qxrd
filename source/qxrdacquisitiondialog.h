#ifndef QXRDACQUISITIONDIALOG_H
#define QXRDACQUISITIONDIALOG_H

#include "qcepmacros.h"
#include "ui_qxrdacquisitiondialog.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"

class QxrdAcquisitionDialog : public QDockWidget, public Ui::QxrdAcquisitionDialog {
  Q_OBJECT
public:
  QxrdAcquisitionDialog(QxrdExperimentWPtr doc,
                    QxrdWindowWPtr win,
                    QxrdAcquisitionWPtr acq,
                    QxrdDataProcessorWPtr proc,
                    QWidget *parent);
  ~QxrdAcquisitionDialog();

  void onAcquisitionInit();

  void acquisitionReady();
  void setupAcquireMenu(QMenu *menu);

protected:
  void changeEvent(QEvent *e);

public slots:
  void doAcquire();
  void doAcquireDark();
  void doCancel();
  void acquireStarted();
  void acquireComplete();

private:
  QxrdExperimentWPtr    m_Experiment;
  QxrdWindowWPtr        m_Window;
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdDataProcessorWPtr m_DataProcessor;
};

#endif // QXRDACQUISITIONDIALOG_H
