#ifndef QXRDDISPLAYDIALOG_H
#define QXRDDISPLAYDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrddisplaydialog.h"
#include "qxrdmainwindow-ptr.h"
#include "qxrdimageplotwidget-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacquisition-ptr.h"

class QXRD_EXPORT QxrdDisplayDialog : public QDialog, public Ui::QxrdDisplayDialog
{
  Q_OBJECT

public:
  explicit QxrdDisplayDialog(QWidget *parent,
                             QxrdExperimentWPtr exptp,
                             QxrdAcquisitionWPtr acqp,
                             QxrdMainWindowWPtr winp,
                             QxrdImagePlotWidgetWPtr plotp);
  ~QxrdDisplayDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdExperimentWPtr      m_Experiment;
  QxrdAcquisitionWPtr     m_Acquisition;
  QxrdMainWindowWPtr      m_MainWindow;
  QxrdImagePlotWidgetWPtr m_PlotWidget;
};

#endif // QXRDDISPLAYDIALOG_H
