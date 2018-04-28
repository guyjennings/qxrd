#ifndef QXRDSIMULATEDDIALOG_H
#define QXRDSIMULATEDDIALOG_H

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdsimulateddialog.h"
#include "qxrdsimulatedsettings-ptr.h"

class QXRD_EXPORT QxrdSimulatedDialog : public QxrdDetectorDialog, public Ui::QxrdSimulatedDialog
{
  Q_OBJECT

public:
  explicit QxrdSimulatedDialog(
      QxrdSimulatedSettingsWPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdSimulatedSettingsWPtr m_Settings;
};

#endif // QXRDSIMULATEDDIALOG_H
