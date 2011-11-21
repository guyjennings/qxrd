#ifndef QXRDWELCOMEWINDOW_H
#define QXRDWELCOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
    class QxrdWelcomeWindow;
}

class QxrdWelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QxrdWelcomeWindow(QWidget *parent = 0);
    ~QxrdWelcomeWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdWelcomeWindow *ui;
};

#endif // QXRDWELCOMEWINDOW_H
