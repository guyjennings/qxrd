#ifndef QXRDFILEWATCHERDIALOG_H
#define QXRDFILEWATCHERDIALOG_H

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdfilewatcherdialog.h"
#include "qxrdfilewatchersettings-ptr.h"

class QXRD_EXPORT QxrdFileWatcherDialog : public QxrdDetectorDialog, public Ui::QxrdFileWatcherDialog
{
  Q_OBJECT

public:
  explicit QxrdFileWatcherDialog(
      QxrdFileWatcherSettingsWPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdFileWatcherSettingsWPtr m_Settings;
};

#endif // QXRDFILEWATCHERDIALOG_H
