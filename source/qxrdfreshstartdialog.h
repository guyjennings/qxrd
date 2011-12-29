#ifndef QXRDFRESHSTARTDIALOG_H
#define QXRDFRESHSTARTDIALOG_H

#include <QDialog>
#include "ui_qxrdfreshstartdialog.h"
#include <QVector>
#include <QCheckBox>

class QxrdFreshStartDialog : public QDialog, public Ui::QxrdFreshStartDialog
{
  Q_OBJECT

public:
  explicit QxrdFreshStartDialog(QWidget *parent = 0);
  ~QxrdFreshStartDialog();

public slots:
  void accept();

protected:
  void changeEvent(QEvent *e);
  void setupDebugWidgets(int dbg);
  int readDebugWidgets();

private:
  QVector<QCheckBox*> m_DebugWidgetList;
};

#endif // QXRDFRESHSTARTDIALOG_H
