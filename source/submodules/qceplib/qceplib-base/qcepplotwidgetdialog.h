#ifndef QCEPPLOTWIDGETDIALOG_H
#define QCEPPLOTWIDGETDIALOG_H

#include "qceplib_global.h"
#include <QDialog>
#include "ui_qcepplotwidgetdialog.h"
#include "qcepplotwidgetsettings-ptr.h"

class QCEP_EXPORT QcepPlotWidgetDialog : public QDialog, public Ui::QcepPlotWidgetDialog
{
  Q_OBJECT

public:
  explicit QcepPlotWidgetDialog(QWidget *parent,
                                QcepPlotWidgetSettingsWPtr settings);
  ~QcepPlotWidgetDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  QcepPlotWidgetSettingsWPtr  m_Settings;
};

#endif // QCEPPLOTWIDGETDIALOG_H
