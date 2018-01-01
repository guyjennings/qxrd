#ifndef QXRDPLOTWIDGETDIALOG_H
#define QXRDPLOTWIDGETDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrdplotwidgetdialog.h"
#include "qxrdplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdPlotWidgetDialog : public QDialog, public Ui::QxrdPlotWidgetDialog
{
  Q_OBJECT

public:
  explicit QxrdPlotWidgetDialog(QWidget *parent,
                                QxrdPlotWidgetSettingsWPtr settings);
  ~QxrdPlotWidgetDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  QxrdPlotWidgetSettingsWPtr  m_Settings;
};

#endif // QXRDPLOTWIDGETDIALOG_H
