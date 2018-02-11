#ifndef QXRDACQUISITIONEXTRAINPUTSDIALOG_H
#define QXRDACQUISITIONEXTRAINPUTSDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "qxrdacqcommon.h"
#include "qxrdacquisitionextrainputs.h"
#include "ui_qxrdacquisitionextrainputsdialog.h"
#include "qxrdacquisitionextrainputsdialogsettings-ptr.h"

class QXRD_EXPORT QxrdAcquisitionExtraInputsDialog : public QDockWidget, public Ui::QxrdAcquisitionExtraInputsDialog
{
  Q_OBJECT
  
public:
  explicit QxrdAcquisitionExtraInputsDialog(QxrdAcquisitionExtraInputsDialogSettingsWPtr set,
                                            QWidget *parent,
                                            QxrdAcqCommonWPtr acq);
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
  QxrdAcqCommonWPtr              m_Acquisition;
  QxrdAcquisitionExtraInputsWPtr m_AcquisitionExtraInputs;
  int                            m_ChannelsInRows;
};

#endif // QXRDACQUISITIONEXTRAINPUTSDIALOG_H
