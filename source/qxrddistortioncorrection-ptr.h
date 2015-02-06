#ifndef QXRDDISTORTIONCORRECTION_PTR_H
#define QXRDDISTORTIONCORRECTION_PTR_H

#include <QSharedPointer>

class QxrdDistortionCorrection;

typedef QSharedPointer<QxrdDistortionCorrection> QxrdDistortionCorrectionPtr;
typedef QWeakPointer<QxrdDistortionCorrection>   QxrdDistortionCorrectionWPtr;

#endif // QXRDDISTORTIONCORRECTION_PTR_H
