#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOG_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOG_H

#include <QDockWidget>
#include "qxrdacquisition.h"
#include "qxrdacquisitionextrainputs.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"
#include "qxrdacquisitionextrainputsdialogsettings.h"

class QxrdAcquisitionExtraInputsDialog : public QDockWidget, public Ui::QxrdAcquisitionExtraInputsDialog
{
  Q_OBJECT
  
public:
  explicit QxrdAcquisitionExtraInputsDialog(QxrdAcquisitionExtraInputsDialogSettingsWPtr set, QWidget *parent, QxrdAcquisitionWPtr acq);
  ~QxrdAcquisitionExtraInputsDialog();

private:
  void setupUiChannel(int i, QxrdAcquisitionExtraInputsChannelPtr ch);

private slots:
  void updateUi();
  void addChannel();
  void removeChannel();
  void initiateReadout();
  void updateWaveforms();

private:
  QxrdAcquisitionExtraInputsDialogSettingsWPtr m_Settings;
  QxrdAcquisitionWPtr            m_Acquisition;
  QxrdAcquisitionExtraInputsWPtr m_AcquisitionExtraInputs;
  int                            m_ChannelsInRows;
};

#endif // QXRDACQUISITIONEXTRAINPUTSDIALOG_H
