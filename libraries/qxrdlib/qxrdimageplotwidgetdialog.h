#ifndef QXRDIMAGEPLOTWIDGETDIALOG_H
#define QXRDIMAGEPLOTWIDGETDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qxrdimageplotwidgetdialog.h"
#include "qxrdimageplotwidgetsettings-ptr.h"

class QXRD_EXPORT QxrdImagePlotWidgetDialog : public QDialog, public Ui::QxrdImagePlotWidgetDialog
{
  Q_OBJECT

public:
  explicit QxrdImagePlotWidgetDialog(QWidget *parent,
                                     QxrdImagePlotWidgetSettingsWPtr settings);
  ~QxrdImagePlotWidgetDialog();

  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  void setMaxPct(double v);
  void setMaxPctle(double v);

private:
  QxrdImagePlotWidgetSettingsWPtr m_Settings;
};

#endif // QXRDIMAGEPLOTWIDGETDIALOG_H
