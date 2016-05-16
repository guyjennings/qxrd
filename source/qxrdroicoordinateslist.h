#ifndef QXRDROICOORDINATESLIST_H
#define QXRDROICOORDINATESLIST_H

#include "qcepobject.h"

class QxrdROICoordinatesList : public QcepObject
{
public:
  Q_INVOKABLE QxrdROICoordinatesList(QString name);
};

Q_DECLARE_METATYPE(QxrdROICoordinatesList*)

#endif // QXRDROICOORDINATESLIST_H
