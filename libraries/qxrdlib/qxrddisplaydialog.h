#ifndef QXRDDISPLAYDIALOG_H
#define QXRDDISPLAYDIALOG_H

#include "qxrdlib_global.h"
#include <QDockWidget>
#include "ui_qxrddisplaydialog.h"

class QXRD_EXPORT QxrdDisplayDialog : public QDockWidget, public Ui::QxrdDisplayDialog
{
    Q_OBJECT

public:
    explicit QxrdDisplayDialog(QWidget *parent = 0);
    ~QxrdDisplayDialog();

protected:
    void changeEvent(QEvent *e);
};

#endif // QXRDDISPLAYDIALOG_H
