#ifndef QXRDGLOBALPREFERENCESDIALOG_H
#define QXRDGLOBALPREFERENCESDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include "ui_qxrdglobalpreferencesdialog.h"

class QxrdApplication;

class QxrdGlobalPreferencesDialog : public QDialog, public Ui::QxrdGlobalPreferencesDialog
{
  Q_OBJECT

public:
  explicit QxrdGlobalPreferencesDialog(QxrdApplication *app, QWidget *parent = 0);
  ~QxrdGlobalPreferencesDialog();

public slots:
  void accept();

protected:
  void changeEvent(QEvent *e);

private:
  void setupDebugWidgets(int dbg);
  int readDebugWidgets();

private:
  QxrdApplication                 *m_Application;
  QVector<QCheckBox*>              m_DebugWidgetList;
};

#endif // QXRDGLOBALPREFERENCESDIALOG_H
