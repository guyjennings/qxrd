#ifndef QXRDCALIBRANTDIALOG_H
#define QXRDCALIBRANTDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrdcalibrantdialog.h"
#include "qxrdcalibrantlibrary-ptr.h"
#include "qxrdcalibrant-ptr.h"
#include "qxrdcalibrantdspacings-ptr.h"
#include "qxrdcalibrantdspacingsmodel-ptr.h"
#include "qxrdcalibrantlibrarymodel-ptr.h"
#include "qxrdcalibrant.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdCalibrantDialog : public QDockWidget, public Ui::QxrdCalibrantDialog
{
  Q_OBJECT

public:
  QxrdCalibrantDialog(QxrdExperimentWPtr expt, QxrdCenterFinderWPtr cf);
  virtual ~QxrdCalibrantDialog();

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

private:
  QxrdExperimentWPtr              m_Experiment;
  QxrdCenterFinderWPtr            m_CenterFinder;
  QxrdCalibrantLibraryPtr         m_CalibrantLibrary;
  QxrdCalibrantLibraryModelPtr    m_CalibrantLibraryModel;
  QxrdCalibrantDSpacingsModelPtr  m_CalibrantDSpacingsModel;
  QxrdCalibrantDSpacingsPtr       m_CalibrantDSpacings;
};

#endif // QXRDCALIBRANTDIALOG_H
