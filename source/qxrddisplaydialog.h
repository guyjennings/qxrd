#ifndef QXRDDISPLAYDIALOG_H
#define QXRDDISPLAYDIALOG_H

#include <QDockWidget>
#include "ui_qxrddisplaydialog.h"

class QxrdDisplayDialog : public QDockWidget, public Ui::QxrdDisplayDialog
{
    Q_OBJECT

public:
    explicit QxrdDisplayDialog(QWidget *parent = 0);
    ~QxrdDisplayDialog();

protected:
    void changeEvent(QEvent *e);
};

#endif // QXRDDISPLAYDIALOG_H

class QxrdDisplayDialog;
