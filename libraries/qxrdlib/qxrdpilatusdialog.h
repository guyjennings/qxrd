#ifndef QXRDPILATUSDIALOG_H
#define QXRDPILATUSDIALOG_H

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdpilatusdialog.h"
#include "qxrdpilatussettings-ptr.h"

class QXRD_EXPORT QxrdPilatusDialog : public QxrdDetectorDialog, public Ui::QxrdPilatusDialog
{
  Q_OBJECT

public:
  explicit QxrdPilatusDialog(
      QxrdPilatusSettingsWPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdPilatusSettingsWPtr m_Settings;
};

#endif // QXRDPILATUSDIALOG_H
