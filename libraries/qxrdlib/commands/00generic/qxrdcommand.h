#ifndef QXRDCOMMAND_H
#define QXRDCOMMAND_H

#include <QObject>
#include "qcepserializableobject.h"

class QxrdCommand : public QcepSerializableObject
{
public:
  QxrdCommand(QString name);
};

#endif // QXRDCOMMAND_H
