#ifndef QXRDTESTDOCKWIDGET_H
#define QXRDTESTDOCKWIDGET_H

#include <QDockWidget>
#include "ui_qxrdtestdockwidget.h"

class QxrdTestDockWidget : public QDockWidget, public Ui::QxrdTestDockWidget
{
    Q_OBJECT

public:
    explicit QxrdTestDockWidget(QWidget *parent = 0);
    ~QxrdTestDockWidget();

protected:
    void changeEvent(QEvent *e);
};

#endif // QXRDTESTDOCKWIDGET_H
