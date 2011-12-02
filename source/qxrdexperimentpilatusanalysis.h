#ifndef QXRDPILATUSANALYSISDOCUMENT_H
#define QXRDPILATUSANALYSISDOCUMENT_H

#include "qxrdanalysisdocument.h"

class QxrdPilatusAnalysisDocument : public QxrdAnalysisDocument
{
    Q_OBJECT
public:
    explicit QxrdPilatusAnalysisDocument(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDPILATUSANALYSISDOCUMENT_H
