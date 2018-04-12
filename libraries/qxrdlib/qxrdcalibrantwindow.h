#ifndef QXRDCALIBRANTWINDOW_H
#define QXRDCALIBRANTWINDOW_H

#include "qcepmacros.h"
#include "qxrdmainwindow.h"
#include "ui_qxrdcalibrantwindow.h"
#include "qcepcalibrant-ptr.h"
#include "qcepcalibrantlibrary-ptr.h"
#include "qcepcalibrantlibrarymodel-ptr.h"
#include "qcepcalibrantdspacings-ptr.h"
#include "qcepcalibrantdspacingsmodel-ptr.h"
#include "qcepcenterfinder-ptr.h"
#include "qxrdcalibrantwindowsettings-ptr.h"

//TODO: implement
class QXRD_EXPORT QxrdCalibrantWindow : public QxrdMainWindow, public Ui::QxrdCalibrantWindow
{
  Q_OBJECT

private:
  typedef QxrdMainWindow inherited;

public:
  explicit QxrdCalibrantWindow(QString name);
  void initialize(QcepObjectWPtr parent);
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
  QcepCenterFinderWPtr            m_CenterFinder;
  QcepCalibrantLibraryPtr         m_CalibrantLibrary;
  QcepCalibrantLibraryModelPtr    m_CalibrantLibraryModel;
  QcepCalibrantDSpacingsModelPtr  m_CalibrantDSpacingsModel;
  QcepCalibrantDSpacingsPtr       m_CalibrantDSpacings;
};

#endif // QXRDCALIBRANTWINDOW_H
