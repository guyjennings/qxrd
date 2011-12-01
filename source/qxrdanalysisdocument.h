#ifndef QXRDANALYSISDOCUMENT_H
#define QXRDANALYSISDOCUMENT_H

#include "qxrddocument.h"

class QxrdAnalysisDocument : public QxrdDocument
{
    Q_OBJECT
public:
    explicit QxrdAnalysisDocument(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDANALYSISDOCUMENT_H
