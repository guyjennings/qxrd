#ifndef QXRDACQUISITIONWINDOW_H
#define QXRDACQUISITIONWINDOW_H

#include "qcepdebug.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdacquisitionwindow.h"
#include "qxrdacquisitionwindowsettings-ptr.h"
#include "qxrddetectorlistmodel-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdacquisitioneventlogwindow-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdAcquisitionWindow : public QxrdMainWindow, public Ui::QxrdAcquisitionWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdAcquisitionWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  ~QxrdAcquisitionWindow();

private slots:
  void doEditCorrection();
  void acquireStarted();
  void acquireComplete();
  void browseLogFile();
  void browseScanFile();

  void eventLogWindow();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdAcquisitionWindowSettingsWPtr m_Settings;
  QxrdExperimentWPtr                m_Experiment;
  QxrdAcqCommonWPtr                 m_Acquisition;
  QxrdProcessorWPtr                 m_Processor;
  QxrdDetectorListModelPtr          m_DetectorsModel;
  QxrdAcquisitionEventLogWindowPtr  m_AcquisitionEventLog;
};

#endif // QXRDACQUISITIONWINDOW_H
