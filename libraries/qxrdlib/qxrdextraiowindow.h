#ifndef QXRDEXTRAIOWINDOW_H
#define QXRDEXTRAIOWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdextraiowindow.h"
#include "qxrdacquisition-ptr.h"
#include "qxrdsynchronizedacquisition-ptr.h"
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
  void waveformChanged();

private slots:
  void initiateReadout();
  void updateWaveforms();
  void updateInputWaveforms();
  void updateOutputWaveforms();

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
  QxrdExtraIODetectorsModelPtr    m_DetectorsModel;
  QxrdExtraIOOutputsModelPtr      m_OutputsModel;
  QxrdExtraIOInputsModelPtr       m_InputsModel;
};

#endif // QXRDEXTRAIOWINDOW_H
