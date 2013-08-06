#include "qcepobjectnamer.h"

QcepObjectNamer::QcepObjectNamer(QObject *object, QString name)
{
  if (object) {
    object->setObjectName(name);
  }
}
