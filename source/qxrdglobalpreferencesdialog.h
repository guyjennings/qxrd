#ifndef QXRDGLOBALPREFERENCESDIALOG_H
#define QXRDGLOBALPREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
    class QxrdGlobalPreferencesDialog;
}

class QxrdGlobalPreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QxrdGlobalPreferencesDialog(QWidget *parent = 0);
    ~QxrdGlobalPreferencesDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdGlobalPreferencesDialog *ui;
};

#endif // QXRDGLOBALPREFERENCESDIALOG_H
