#ifndef QXRDCALIBRANTDIALOG_H
#define QXRDCALIBRANTDIALOG_H

#include <QDockWidget>
#include "ui_qxrdcalibrantdialog.h"
#include "qxrdcalibrantlibrary-ptr.h"
#include "qxrdcalibrant-ptr.h"
#include "qxrdcalibrantdspacings.h"
#include "qxrdcalibrantdspacingsmodel-ptr.h"
#include "qxrdcalibrantlibrarymodel-ptr.h"
#include "qxrdcalibrant.h"
#include "qxrdcenterfinder-ptr.h"

class QxrdCalibrantDialog : public QDockWidget, public Ui::QxrdCalibrantDialog
{
  Q_OBJECT

public:
  QxrdCalibrantDialog(QxrdExperimentPtr expt, QxrdCenterFinderWPtr cf);
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
  QxrdCalibrantLibraryWPtr        m_CalibrantLibrary;
  QxrdCalibrantLibraryModelWPtr   m_CalibrantLibraryModel;
  QxrdCalibrantDSpacingsModelWPtr m_CalibrantDSpacingsModel;
  QxrdCalibrantDSpacingsWPtr      m_CalibrantDSpacings;
};

#endif // QXRDCALIBRANTDIALOG_H
