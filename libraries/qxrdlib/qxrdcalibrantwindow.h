#ifndef QXRDCALIBRANTWINDOW_H
#define QXRDCALIBRANTWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcalibrantwindow.h"
#include "qxrdcalibrant-ptr.h"
#include "qxrdcalibrantlibrary-ptr.h"
#include "qxrdcalibrantlibrarymodel-ptr.h"
#include "qxrdcalibrantdspacings-ptr.h"
#include "qxrdcalibrantdspacingsmodel-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdcalibrantwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdCalibrantWindow : public QxrdMainWindow, public Ui::QxrdCalibrantWindow
{
  Q_OBJECT

public:
  explicit QxrdCalibrantWindow(QxrdCalibrantWindowSettingsWPtr set,
                               QString name,
                               QxrdApplicationWPtr app,
                               QxrdExperimentWPtr expt,
                               QxrdAcquisitionWPtr acqw,
                               QxrdProcessorWPtr procw);
  ~QxrdCalibrantWindow();

public slots:
  void calibrantTableContextMenu(const QPoint &pos);
  void calibrantDSpacingsContextMenu(const QPoint &pos);

  void onCalibrantChanged();

  void onCalibrantClick      (const QModelIndex &item);
  void onCalibrantDoubleClick(const QModelIndex &item);

private:
  void doCopyFromTable(QTableView *table);
  void doDuplicateCalibrant(int n);
  void doDeleteCalibrant(int n);
  void doCalibrantProperties(int n);

protected:
  void changeEvent(QEvent *e);

private:
  QxrdCalibrantWindowSettingsWPtr m_CalibrantWindowSettings;
  QxrdCenterFinderWPtr            m_CenterFinder;
  QxrdCalibrantLibraryPtr         m_CalibrantLibrary;
  QxrdCalibrantLibraryModelPtr    m_CalibrantLibraryModel;
  QxrdCalibrantDSpacingsModelPtr  m_CalibrantDSpacingsModel;
  QxrdCalibrantDSpacingsPtr       m_CalibrantDSpacings;
};

#endif // QXRDCALIBRANTWINDOW_H
