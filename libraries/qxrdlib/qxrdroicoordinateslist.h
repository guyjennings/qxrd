#ifndef QXRDROICOORDINATESLIST_H
#define QXRDROICOORDINATESLIST_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

//TODO: rename QxrdROIVector
class QXRD_EXPORT QxrdROICoordinatesList : public QcepObject
{
public:
  Q_INVOKABLE QxrdROICoordinatesList(QString name);
};

Q_DECLARE_METATYPE(QxrdROICoordinatesList*)

#endif // QXRDROICOORDINATESLIST_H
