#ifndef QXRDPERKINELMERDIALOG_H
#define QXRDPERKINELMERDIALOG_H

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdperkinelmerdialog.h"
#include "qxrdperkinelmersettings-ptr.h"

class QXRD_EXPORT QxrdPerkinElmerDialog : public QxrdDetectorDialog, public Ui::QxrdPerkinElmerDialog
{
  Q_OBJECT

public:
  explicit QxrdPerkinElmerDialog(
      QxrdPerkinElmerSettingsPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdPerkinElmerSettingsPtr m_Settings;
};

#endif // QXRDPERKINELMERDIALOG_H
