#ifndef QXRDACQUISITIONWINDOW_H
#define QXRDACQUISITIONWINDOW_H

#include "qcepdebug.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdacquisitionwindow.h"
#include "qxrdacquisitionwindowsettings-ptr.h"

class QXRD_EXPORT QxrdAcquisitionWindow : public QxrdMainWindow, public Ui::QxrdAcquisitionWindow
{
  Q_OBJECT

public:
  explicit QxrdAcquisitionWindow(QxrdAcquisitionWindowSettingsWPtr set,
                                 QString name,
                                 QxrdApplicationWPtr app,
                                 QxrdExperimentWPtr expt);
  ~QxrdAcquisitionWindow();

private slots:
  void doEditCorrection();

protected:
  void changeEvent(QEvent *e);

  QxrdAcquisitionWindowSettingsWPtr m_AcquisitionWindowSettings;
};

#endif // QXRDACQUISITIONWINDOW_H
