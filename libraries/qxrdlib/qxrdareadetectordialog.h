#ifndef QXRDAREADETECTORDIALOG_H
#define QXRDAREADETECTORDIALOG_H

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdareadetectordialog.h"
#include "qxrdareadetectorsettings-ptr.h"

class QXRD_EXPORT QxrdAreaDetectorDialog : public QxrdDetectorDialog, public Ui::QxrdAreaDetectorDialog
{
  Q_OBJECT

public:
  explicit QxrdAreaDetectorDialog(
      QxrdAreaDetectorSettingsPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdAreaDetectorSettingsPtr m_Settings;
};

#endif // QXRDAREADETECTORDIALOG_H
