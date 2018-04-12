#ifndef QXRDCALIBRANTPROPERTIESDIALOG_H
#define QXRDCALIBRANTPROPERTIESDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include "ui_qxrdcalibrantpropertiesdialog.h"
#include "qcepcalibrant-ptr.h"

class QXRD_EXPORT QxrdCalibrantPropertiesDialog : public QDialog, public Ui::QxrdCalibrantPropertiesDialog
{
  Q_OBJECT

public:
  explicit QxrdCalibrantPropertiesDialog(QWidget *parent, QcepCalibrantPtr cal);
  ~QxrdCalibrantPropertiesDialog();

  void accept();

private:
  QcepCalibrantPtr m_Calibrant;
};

#endif // QXRDCALIBRANTPROPERTIESDIALOG_H
