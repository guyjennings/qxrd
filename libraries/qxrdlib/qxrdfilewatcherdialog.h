#ifndef QXRDFILEWATCHERDIALOG_H
#define QXRDFILEWATCHERDIALOG_H

//TODO: recast to multiple inheritance
//TODO: make smart pointer

#include "qxrdlib_global.h"
#include "qxrddetectordialog.h"
#include "ui_qxrdfilewatcherdialog.h"
#include "qxrdfilewatchersettings-ptr.h"

class QXRD_EXPORT QxrdFileWatcherDialog : public QxrdDetectorDialog, public Ui::QxrdFileWatcherDialog
{
  Q_OBJECT

public:
  explicit QxrdFileWatcherDialog(
      QxrdFileWatcherSettingsPtr set,
      QWidget *parent = 0);

  void accept();

private:
  QxrdFileWatcherSettingsPtr m_Settings;
};

#endif // QXRDFILEWATCHERDIALOG_H
