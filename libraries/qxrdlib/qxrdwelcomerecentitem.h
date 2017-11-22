#ifndef QXRDWELCOMERECENTITEM_H
#define QXRDWELCOMERECENTITEM_H

#include "qxrdlib_global.h"
#include <QCommandLinkButton>
#include "ui_qxrdwelcomerecentitem.h"

class QXRD_EXPORT QxrdWelcomeRecentItem : public QCommandLinkButton, public Ui::QxrdWelcomeRecentItem
{
    Q_OBJECT

public:
    explicit QxrdWelcomeRecentItem(QWidget *parent = 0);
    ~QxrdWelcomeRecentItem();

protected:
    void changeEvent(QEvent *e);
};

#endif // QXRDWELCOMERECENTITEM_H
