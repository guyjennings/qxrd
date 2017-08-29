#ifndef QXRDACQUISITIONDIALOG_H
#define QXRDACQUISITIONDIALOG_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "ui_qxrdacquisitiondialog.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"

class QXRD_EXPORT QxrdAcquisitionDialog : public QDockWidget, public Ui::QxrdAcquisitionDialog {
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
  void setupDetectorsMenu(QMenu *menu);

protected:
  void changeEvent(QEvent *e);

public slots:
  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();
  void doCancel();
  void acquireStarted();
  void acquireComplete();

  void browseOutputDirectory();
  void browseLogFile();
  void browseScanFile();

private:
  QxrdExperimentWPtr    m_Experiment;
  QxrdWindowWPtr        m_Window;
  QxrdAcquisitionWPtr   m_Acquisition;
  QxrdDataProcessorWPtr m_DataProcessor;
};

#endif // QXRDACQUISITIONDIALOG_H
