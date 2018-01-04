#ifndef QXRDACQUISITIONWINDOW_H
#define QXRDACQUISITIONWINDOW_H

#include "qcepdebug.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdacquisitionwindow.h"
#include "qxrdacquisitionwindowsettings-ptr.h"
#include "qxrddetectorlistmodel-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdapplication-ptr.h"
#include "qxrdacquisition-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdAcquisitionWindow : public QxrdMainWindow, public Ui::QxrdAcquisitionWindow
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionWindow(QxrdAcquisitionWindowSettingsWPtr set,
                                 QString name,
                                 QxrdApplicationWPtr app,
                                 QxrdExperimentWPtr expt,
                                 QxrdAcquisitionWPtr acqw,
                                 QxrdProcessorWPtr  procw);
  ~QxrdAcquisitionWindow();

private slots:
  void doEditCorrection();
  void acquireStarted();
  void acquireComplete();
  void browseLogFile();
  void browseScanFile();

protected:
  void changeEvent(QEvent *e);

  QxrdAcquisitionWindowSettingsWPtr m_AcquisitionWindowSettings;
  QxrdAcquisitionWPtr      m_Acquisition;
  QxrdDetectorListModelPtr m_DetectorsModel;
};

#endif // QXRDACQUISITIONWINDOW_H
