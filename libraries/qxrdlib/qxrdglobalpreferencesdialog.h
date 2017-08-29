#ifndef QXRDGLOBALPREFERENCESDIALOG_H
#define QXRDGLOBALPREFERENCESDIALOG_H

#include "qxrdlib_global.h"
#include <QDialog>
#include <QCheckBox>
#include "ui_qxrdglobalpreferencesdialog.h"
#include "qxrdapplicationsettings-ptr.h"

class QXRD_EXPORT QxrdGlobalPreferencesDialog : public QDialog, public Ui::QxrdGlobalPreferencesDialog
{
  Q_OBJECT

public:
  explicit QxrdGlobalPreferencesDialog(QxrdApplicationSettingsWPtr set);
  ~QxrdGlobalPreferencesDialog();

public slots:
  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  void setupDebugWidgets(qint64 dbg);
  qint64 readDebugWidgets();

private:
  QxrdApplicationSettingsPtr       m_ApplicationSettings;
  QVector<QCheckBox*>              m_DebugWidgetList;
};

#endif // QXRDGLOBALPREFERENCESDIALOG_H
