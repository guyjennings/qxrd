#ifndef QXRDEXTRAIOWINDOW_H
#define QXRDEXTRAIOWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdextraiowindow.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
#include "qxrdacquisitionextrainputs-ptr.h"
#include "qxrdacquisitionextrainputschannel-ptr.h"
#include "qxrdextraiowindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdExtraIOWindow : public QxrdMainWindow, public Ui::QxrdExtraIOWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdExtraIOWindow(QxrdExtraIOWindowSettingsWPtr set,
                             QString name,
                             QxrdAppCommonWPtr app,
                             QxrdExperimentWPtr expt,
                             QxrdAcqCommonWPtr acqw,
                             QxrdProcessorWPtr procw);
  void initialize(QcepObjectWPtr parent);
  ~QxrdExtraIOWindow();

protected:
  void changeEvent(QEvent *e);

private slots:
  void deviceChanged();
  void waveformChanged();

private:
  void setupUiChannel(int i, QxrdAcquisitionExtraInputsChannelPtr ch);

private slots:
  void updateUi();
  void addChannel();
  void removeChannel();
  void initiateReadout();
  void updateWaveforms();

private:
  QxrdExtraIOWindowSettingsWPtr   m_ExtraIOWindowSettings;
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;
  QxrdAcquisitionExtraInputsWPtr m_AcquisitionExtraInputs;
  int                            m_ChannelsInRows;
};

#endif // QXRDEXTRAIOWINDOW_H
