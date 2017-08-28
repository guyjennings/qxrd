#ifndef QCEPOBJECTNAMER_H
#define QCEPOBJECTNAMER_H

#include "qceplib_global.h"
#include <QObject>
#include <QString>

class QCEP_EXPORT QcepObjectNamer
{
public:
  QcepObjectNamer(QObject *object, QString name);
};

#endif // QCEPOBJECTNAMER_H
