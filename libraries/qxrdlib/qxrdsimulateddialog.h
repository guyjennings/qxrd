#ifndef QXRDSIMULATEDDIALOG_H
#define QXRDSIMULATEDDIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdsimulateddialog.h"
#include "qxrdsimulatedsettings-ptr.h"

class QXRD_EXPORT QxrdSimulatedDialog : public QxrdDetectorDialog, public Ui::QxrdSimulatedDialog
{
  Q_OBJECT

public:
  explicit QxrdSimulatedDialog(
      QxrdSimulatedSettingsPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdSimulatedSettingsPtr m_Settings;
};

#endif // QXRDSIMULATEDDIALOG_H
