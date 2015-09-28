#ifndef QXRDDETECTORCONTROLWINDOW_H
#define QXRDDETECTORCONTROLWINDOW_H

#include "qxrdmainwindow.h"
#include "qcepsettingssaver-ptr.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetectorprocessor-ptr.h"
#include "qxrdroicoordinateslistmodel-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdDetectorControlWindow : public QxrdMainWindow, public Ui::QxrdDetectorControlWindow
{
  Q_OBJECT

public:
  explicit QxrdDetectorControlWindow(QcepSettingsSaverWPtr     saver,
                                     QxrdExperimentWPtr        exp,
                                     QxrdAcquisitionWPtr       acq,
                                     QxrdDetectorProcessorWPtr proc,
                                     QWidget                  *parent = 0);
  ~QxrdDetectorControlWindow();

public slots:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

protected:
  void changeEvent(QEvent *e);

private slots:
  void doAppendROI();
  void doDeleteROI();
  void doMoveROIDown();
  void doMoveROIUp();

private:
  QVector<int>  selectedROIs();

private:
  QcepSettingsSaverWPtr           m_Saver;
  QxrdExperimentWPtr              m_Experiment;
  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdDetectorProcessorWPtr       m_Processor;
  QxrdROICoordinatesListModelPtr  m_ROIModel;
};

#endif // QXRDDETECTORCONTROLWINDOW_H
