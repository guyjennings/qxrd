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

private:
  QxrdCalibrantLibraryPtr m_CalibrantLibrary;
  QxrdCalibrantLibraryModelPtr m_CalibrantLibraryModel;
  QxrdCalibrantDSpacingsModelPtr m_CalibrantDSpacingsModel;
  QxrdCalibrantDSpacingVector m_CalibrantDSpacingsVector;
  QxrdCenterFinderWPtr        m_CenterFinder;
};

#endif // QXRDCALIBRANTDIALOG_H
