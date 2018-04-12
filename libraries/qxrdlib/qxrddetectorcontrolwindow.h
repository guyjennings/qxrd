#ifndef QXRDDETECTORCONTROLWINDOW_H
#define QXRDDETECTORCONTROLWINDOW_H

#include "qxrdlib_global.h"
#include "qxrdmainwindow.h"
#include "ui_qxrddetectorcontrolwindow.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qceproimodel-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrddetectorcontrolwindowsettings-ptr.h"
#include <QTimer>
#include "qcepdataobjectpropertiesmodel-ptr.h"
#include "qxrdinfowindow-ptr.h"
#include "qxrdacquisitioneventlogwindow-ptr.h"

#define QXRD_DETECTOR_WINDOW_STATE_VERSION 0

//TODO: implement
class QXRD_EXPORT QxrdDetectorControlWindow : public QxrdMainWindow, public Ui::QxrdDetectorControlWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdDetectorControlWindow(QString name);
  void initialize(QcepObjectWPtr parent);
  virtual ~QxrdDetectorControlWindow();

  void setupMenus(QMenu *file, QMenu *edit, QMenu *window);

public slots:
  virtual void printLine(QString line);
  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

  virtual void displayMessage(QString msg);

  void doAcquire();
  void doAcquireOnce();
  void doAcquireDark();
  void doCancel();
  void acquireStarted();
  void acquireComplete();
  void browseLogFile();
  void browseScanFile();

  void onChangeROICreateType(int newType);

  void newImage(QcepImageDataBasePtr img);

public:
  void displayNewData(QcepImageDataBasePtr img, QcepMaskDataPtr overflow);
  void displayNewMask(QcepMaskDataPtr mask);

  void captureSize();
  void resizeEvent(QResizeEvent *);
  void moveEvent(QMoveEvent *);

  QxrdDetectorControlWindowSettingsWPtr detectorControlWindowSettings();

protected:
  void changeEvent(QEvent *e);

private slots:
  void doAppendROI();
  void doDeleteROI();
  void doMoveROIDown();
  void doMoveROIUp();

  void updateImageDisplay();
  void onUpdateIntervalMsecChanged(int newVal);
  QString windowName() const;

  void updateROIDisplay(bool show);

  void doBrowseMask();

  void doBrowseDark();
  void doClearDark();

  void doBrowseBadPixels();
  void doClearBadPixels();

  void doBrowseGainCorrection();
  void doClearGainCorrection();

  void doEditROI();
  void doRecalculate();
  void doVisualizeBackground();
  void doVisualizePeak();

  void openEventLog();
  void restartDetectors();
  void detectorInfoWindow();

private:
  QVector<int>  selectedROIs();
  void appendROI(int roiType);

private:
  QxrdExperimentWPtr         m_Experiment;
  QxrdAcqCommonWPtr          m_Acquisition;
  QxrdDetectorSettingsWPtr   m_Detector;
  QxrdProcessorWPtr          m_Processor;
  QcepROIModelWPtr           m_ROIModel;

  QcepDataObjectPropertiesModelPtr m_ImageProperties;

  QMutex                     m_UpdateMutex;
  QTimer                     m_UpdateTimer;

  QcepImageDataBasePtr       m_DisplayedImage;
  QcepMaskDataPtr            m_DisplayedMask;
  QcepMaskDataPtr            m_DisplayedOverflow;

  QcepImageDataBasePtr       m_NewImage;
  QcepMaskDataPtr            m_NewMask;
  QcepMaskDataPtr            m_NewOverflow;

  QAtomicInt                 m_NewDataAvailable;
  QAtomicInt                 m_NewMaskAvailable;

  QxrdAcquisitionEventLogWindowPtr m_AcquisitionEventLog;
  QxrdInfoWindowPtr                m_InfoWindow;
};

#endif // QXRDDETECTORCONTROLWINDOW_H
