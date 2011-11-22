#ifndef QXRDWELCOMEWINDOW_H
#define QXRDWELCOMEWINDOW_H

#include <QMainWindow>

class QxrdApplication;

namespace Ui {
    class QxrdWelcomeWindow;
}

class QxrdWelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QxrdWelcomeWindow(QxrdApplication *app);
    ~QxrdWelcomeWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdWelcomeWindow *ui;
};

#endif // QXRDWELCOMEWINDOW_H
