#ifndef QCEPPLOTWIDGETDIALOG_H
#define QCEPPLOTWIDGETDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qcepplotwidgetdialog.h"
#include "qcepplotwidgetsettings-ptr.h"

class QXRD_EXPORT QcepPlotWidgetDialog : public QDialog, public Ui::QcepPlotWidgetDialog
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
