#ifndef QXRDPERKINELMERDOCUMENT_H
#define QXRDPERKINELMERDOCUMENT_H

#include "qxrddocument.h"

class QxrdPerkinElmerDocument : public QxrdDocument
{
    Q_OBJECT
public:
    explicit QxrdPerkinElmerDocument(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDPERKINELMERDOCUMENT_H
