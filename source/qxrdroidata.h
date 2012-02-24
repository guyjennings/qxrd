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

#endif // QXRDROIDATA_H

class QxrdROIData;
typedef QSharedPointer<QxrdROIData> QxrdROIDataPtr;
