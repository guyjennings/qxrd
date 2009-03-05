#include "qxrdsettings.h"

QxrdSettings::QxrdSettings(QObject *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd",parent)
{
}
