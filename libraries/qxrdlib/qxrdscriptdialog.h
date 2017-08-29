#ifndef QXRDSCRIPTDIALOG_H
#define QXRDSCRIPTDIALOG_H

#include <QDockWidget>
#include "ui_qxrdscriptdialog.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdscriptdialogsettings-ptr.h"

class QxrdScriptDialog : public QDockWidget, public Ui::QxrdScriptDialog
{
  Q_OBJECT

public:
  explicit QxrdScriptDialog(QxrdScriptDialogSettingsWPtr settings, QxrdExperimentWPtr experiment, QWidget *parent);
  ~QxrdScriptDialog();

private:
  QxrdScriptDialogSettingsWPtr m_ScriptDialogSettings;
};

#endif // QXRDSCRIPTDIALOG_H
