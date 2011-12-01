#ifndef QXRDSIMULATEDDOCUMENT_H
#define QXRDSIMULATEDDOCUMENT_H

#include "qxrddocument.h"

class QxrdSimulatedDocument : public QxrdDocument
{
    Q_OBJECT
public:
    explicit QxrdSimulatedDocument(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDSIMULATEDDOCUMENT_H
