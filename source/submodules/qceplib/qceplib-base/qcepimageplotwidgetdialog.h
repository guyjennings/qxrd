#ifndef QCEPIMAGEPLOTWIDGETDIALOG_H
#define QCEPIMAGEPLOTWIDGETDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "ui_qcepimageplotwidgetdialog.h"
#include "qcepimageplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepImagePlotWidgetDialog : public QDialog, public Ui::QcepImagePlotWidgetDialog
{
  Q_OBJECT

public:
  explicit QcepImagePlotWidgetDialog(QWidget *parent,
                                     QcepImagePlotWidgetSettingsWPtr settings);
  ~QcepImagePlotWidgetDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  void setMaxPct(double v);
  void setMaxPctle(double v);

private:
  QcepImagePlotWidgetSettingsWPtr m_Settings;
};

#endif // QCEPIMAGEPLOTWIDGETDIALOG_H
