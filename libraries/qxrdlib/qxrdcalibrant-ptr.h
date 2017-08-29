#ifndef QXRDCALIBRANTPTR_H
#define QXRDCALIBRANTPTR_H

#include <QSharedPointer>

class QxrdCalibrant;

typedef QSharedPointer<QxrdCalibrant> QxrdCalibrantPtr;
typedef QWeakPointer<QxrdCalibrant>   QxrdCalibrantWPtr;

#endif // QXRDCALIBRANTPTR_H

