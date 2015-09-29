#ifndef QXRDDETECTORCONTROLWINDOW_H
#define QXRDDETECTORCONTROLWINDOW_H

#include "qxrdmainwindow.h"
#include "qcepsettingssaver-ptr.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetectorprocessor-ptr.h"
#include "qxrdroicoordinateslistmodel-ptr.h"
#include "qxrdexperiment-ptr.h"
#include <QTimer>

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

public:
  void displayNewData(QcepImageDataBasePtr img, QcepMaskDataPtr overflow);
  void displayNewMask(QcepMaskDataPtr mask);

protected:
  void changeEvent(QEvent *e);

private slots:
  void doAppendROI();
  void doDeleteROI();
  void doMoveROIDown();
  void doMoveROIUp();

  void updateImageDisplay();
  void onUpdateIntervalMsecChanged(int newVal);

private:
  QVector<int>  selectedROIs();

private:
  QcepSettingsSaverWPtr           m_Saver;
  QxrdExperimentWPtr              m_Experiment;
  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdDetectorProcessorWPtr       m_Processor;
  QxrdROICoordinatesListModelPtr  m_ROIModel;

  QMutex                          m_UpdateMutex;
  QTimer                          m_UpdateTimer;

  QcepImageDataBasePtr            m_DisplayedImage;
  QcepMaskDataPtr                 m_DisplayedMask;
  QcepMaskDataPtr                 m_DisplayedOverflow;

  QcepImageDataBasePtr            m_NewImage;
  QcepMaskDataPtr                 m_NewMask;
  QcepMaskDataPtr                 m_NewOverflow;

  QAtomicInt                      m_NewDataAvailable;
  QAtomicInt                      m_NewMaskAvailable;
};

#endif // QXRDDETECTORCONTROLWINDOW_H
