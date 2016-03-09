#ifndef QXRDCALIBRANTDIALOG_H
#define QXRDCALIBRANTDIALOG_H

#include <QDockWidget>
#include "ui_qxrdcalibrantdialog.h"
#include "qxrdcalibrantlibrary-ptr.h"
#include "qxrdcalibrant-ptr.h"
#include "qxrdcalibrantdspacingsmodel-ptr.h"
#include "qxrdcalibrantlibrarymodel-ptr.h"
#include "qxrdcalibrant.h"
#include "qxrdcenterfinder-ptr.h"

class QxrdCalibrantDialog : public QDockWidget, public Ui::QxrdCalibrantDialog
{
  Q_OBJECT

public:
  QxrdCalibrantDialog(QxrdCalibrantLibraryPtr cal, QxrdCenterFinderWPtr cf, QWidget *parent = 0);
  virtual ~QxrdCalibrantDialog();

public slots:
  void onLibrarySelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

  void calibrantTableContextMenu(const QPoint &pos);
  void calibrantDSpacingsContextMenu(const QPoint &pos);

  void onCalibrantClick      (const QModelIndex &item);
  void onCalibrantDoubleClick(const QModelIndex &item);

private:
  void doCopyFromTable(QTableView *table);
  void doDuplicateCalibrant(int n);
  void doDeleteCalibrant(int n);
  void doCalibrantProperties(int n);

private:
  QxrdCalibrantLibraryPtr m_CalibrantLibrary;
  QxrdCalibrantLibraryModelPtr m_CalibrantLibraryModel;
  QxrdCalibrantDSpacingsModelPtr m_CalibrantDSpacingsModel;
  QxrdCalibrantDSpacingVector m_CalibrantDSpacingsVector;
  QxrdCenterFinderWPtr        m_CenterFinder;
};

#endif // QXRDCALIBRANTDIALOG_H
