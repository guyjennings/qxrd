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
#include "qxrdextraiodetectorsmodel-ptr.h"
#include "qxrdextraiooutputsmodel-ptr.h"
#include "qxrdextraioinputsmodel-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdExtraIOWindow : public QxrdMainWindow, public Ui::QxrdExtraIOWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdExtraIOWindow(QString name);
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
//  void updateUi();
//  void addChannel();
//  void removeChannel();
  void initiateReadout();
  void updateWaveforms();

  void doNewDetector();
  void doNewOutput();
  void doNewInput();
  void doDeleteDetector();
  void doDeleteOutput();
  void doDeleteInput();

  bool askToInsert(QString thing, int before, int count);
  bool askToDelete(QString thing, QVector<int> items);

private:
  QxrdSynchronizedAcquisitionWPtr m_SynchronizedAcquisition;
  QxrdAcquisitionExtraInputsWPtr  m_AcquisitionExtraInputs;
  int                             m_ChannelsInRows;
  QxrdExtraIODetectorsModelPtr    m_DetectorsModel;
  QxrdExtraIOOutputsModelPtr      m_OutputsModel;
  QxrdExtraIOInputsModelPtr       m_InputsModel;
};

#endif // QXRDEXTRAIOWINDOW_H
