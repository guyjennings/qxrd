#include "qxrdobjectnamer.h"

QxrdObjectNamer::QxrdObjectNamer(QObject *object, QString name)
{
  if (object) {
    object->setObjectName(name);
  }
}

