#ifndef QXRDWELCOMERECENTITEM_H
#define QXRDWELCOMERECENTITEM_H

#include <QCommandLinkButton>

namespace Ui {
    class QxrdWelcomeRecentItem;
}

class QxrdWelcomeRecentItem : public QCommandLinkButton
{
    Q_OBJECT

public:
    explicit QxrdWelcomeRecentItem(QWidget *parent = 0);
    ~QxrdWelcomeRecentItem();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QxrdWelcomeRecentItem *ui;
};

#endif // QXRDWELCOMERECENTITEM_H

class QxrdWelcomeRecentItem;
