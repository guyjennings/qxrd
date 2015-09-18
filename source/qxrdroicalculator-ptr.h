#ifndef QXRDROICALCULATORPTR_H
#define QXRDROICALCULATORPTR_H

#include <QSharedPointer>

class QxrdROICalculator;

typedef QSharedPointer<QxrdROICalculator> QxrdROICalculatorPtr;
typedef QWeakPointer<QxrdROICalculator>   QxrdROICalculatorWPtr;

#endif // QXRDROICALCULATORPTR_H

