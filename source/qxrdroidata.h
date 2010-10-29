#ifndef QXRDROIDATA_H
#define QXRDROIDATA_H

#include "qcepmacros.h"
#include <QObject>
#include <QSharedPointer>

class QxrdROIData : public QObject
{
Q_OBJECT
public:
    explicit QxrdROIData(QObject *parent = 0);

signals:

public slots:
};

typedef QSharedPointer<QxrdROIData> QxrdROIDataPtr;

#endif // QXRDROIDATA_H
