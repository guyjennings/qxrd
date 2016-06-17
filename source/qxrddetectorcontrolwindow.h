#ifndef QXRDDETECTORCONTROLWINDOW_H
#define QXRDDETECTORCONTROLWINDOW_H

#include "qxrdmainwindow.h"
#include "qcepsettingssaver-ptr.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddetectorprocessor-ptr.h"
#include "qxrdroicoordinateslistmodel-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include <QTimer>

class QxrdDetectorControlWindow : public QxrdMainWindow, public Ui::QxrdDetectorControlWindow
{
  Q_OBJECT

public:
  explicit QxrdDetectorControlWindow(QxrdExperimentWPtr        exp,
                                     QxrdAcquisitionWPtr       acq,
                                     QxrdDetectorSettingsWPtr          det,
                                     QxrdDetectorProcessorWPtr proc,
                                     QWidget                  *parent = 0);
  ~QxrdDetectorControlWindow();

public slots:
  virtual void printLine(QString line);
  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  virtual void displayMessage(QString msg);

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
  void updateWindowTitle();

  void updateROIDisplay(bool show);

  void doBrowseMask();
  void doClearMask();

  void doBrowseDark();
  void doClearDark();

  void doBrowseBadPixels();
  void doClearBadPixels();

  void doBrowseGainCorrection();
  void doClearGainCorrection();

  void doRecalculate();
  void doVisualizeBackground();
  void doVisualizePeak();

private:
  QVector<int>  selectedROIs();

private:
  QxrdExperimentWPtr              m_Experiment;
  QxrdAcquisitionWPtr             m_Acquisition;
  QxrdDetectorSettingsWPtr                m_Detector;
  QxrdDetectorProcessorWPtr       m_Processor;
  QxrdROICoordinatesListModelWPtr  m_ROIModel;

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
