#ifndef QXRDALLIEDVISIONDIALOG_H
#define QXRDALLIEDVISIONDIALOG_H

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdalliedvisiondialog.h"
#include "qxrdalliedvisionsettings-ptr.h"

class QXRD_EXPORT QxrdAlliedVisionDialog : public QxrdDetectorDialog, public Ui::QxrdAlliedVisionDialog
{
  Q_OBJECT

public:
  explicit QxrdAlliedVisionDialog(
      QxrdAlliedVisionSettingsWPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdAlliedVisionSettingsWPtr m_Settings;
};

#endif // QXRDALLIEDVISIONDIALOG_H
