#ifndef QXRDDEXELADIALOG_H
#define QXRDDEXELADIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrddexeladialog.h"
#include "qxrddexelasettings-ptr.h"

class QXRD_EXPORT QxrdDexelaDialog : public QxrdDetectorDialog, public Ui::QxrdDexelaDialog
{
  Q_OBJECT

public:
  explicit QxrdDexelaDialog(
      QxrdDexelaSettingsPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdDexelaSettingsPtr m_Settings;
};

#endif // QXRDDEXELADIALOG_H
