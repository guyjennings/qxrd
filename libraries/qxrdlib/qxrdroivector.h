#ifndef QXRDROIVECTOR_H
#define QXRDROIVECTOR_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

//TODO: implement as ROI data store
class QXRD_EXPORT QxrdROIVector : public QcepObject
{
public:
  Q_INVOKABLE QxrdROIVector(QString name);
};

Q_DECLARE_METATYPE(QxrdROIVector*)

#endif // QXRDROIVECTOR_H
