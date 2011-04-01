#include "qxrdsettings.h"
#include <stdio.h>

QxrdSettings::QxrdSettings(QObject *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd-2",parent)
{
  if (qcepDebug(DEBUG_PREFS)) {
    printf("QxrdSettings::QxrdSettings\n");
  }
}

QxrdSettings::QxrdSettings(const QString &path, QSettings::Format format, QObject *parent)
  : QSettings(path, format, parent)
{

}

QxrdSettings::~QxrdSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    printf("QxrdSettings::~QxrdSettings\n");
  }
}
