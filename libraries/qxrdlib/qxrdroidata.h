#ifndef QXRDROIDATA_H
#define QXRDROIDATA_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"

//TODO: eliminate
class QXRD_EXPORT QxrdROIData : public QcepObject
{
Q_OBJECT
public:
    explicit QxrdROIData(QString name);

signals:

public slots:
};

#endif // QXRDROIDATA_H
