#ifndef QXRDGENERICANALYSISDOCUMENT_H
#define QXRDGENERICANALYSISDOCUMENT_H

#include "qxrdanalysisdocument.h"

class QxrdGenericAnalysisDocument : public QxrdAnalysisDocument
{
    Q_OBJECT
public:
    explicit QxrdGenericAnalysisDocument(QString path, QxrdApplication *app, QObject *parent = 0);

signals:

public slots:

};

#endif // QXRDGENERICANALYSISDOCUMENT_H
