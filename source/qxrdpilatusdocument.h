#ifndef QXRDPILATUSDOCUMENT_H
#define QXRDPILATUSDOCUMENT_H

#include "qxrddocument.h"

class QxrdPilatusDocument : public QxrdDocument
{
    Q_OBJECT
public:
    explicit QxrdPilatusDocument(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDPILATUSDOCUMENT_H
