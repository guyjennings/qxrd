#include "qxrdsettings.h"
#include <stdio.h>
#include "qxrdapplication.h"

QxrdSettings::QxrdSettings(QObject *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd-2",parent)
{
  if (qcepDebug(DEBUG_PREFS)) {
    g_Application->printMessage("QxrdSettings::QxrdSettings");
  }
}

QxrdSettings::QxrdSettings(const QString &path, QSettings::Format format, QObject *parent)
  : QSettings(path, format, parent)
{

}

QxrdSettings::~QxrdSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    g_Application->printMessage("QxrdSettings::~QxrdSettings");
  }
}
