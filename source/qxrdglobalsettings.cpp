#include "qxrdglobalsettings.h"
#include <stdio.h>
#include "qxrdapplication.h"

QxrdGlobalSettings::QxrdGlobalSettings(QxrdApplication *app)
  : QSettings("cep.xor.aps.anl.gov","qxrd-2",NULL),
    m_Application(app)
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Application->printMessage("QxrdGlobalSettings::QxrdGlobalSettings");
  }
}

QxrdGlobalSettings::QxrdGlobalSettings(const QString &path, QSettings::Format format, QObject *parent)
  : QSettings(path, format, parent)
{

}

QxrdGlobalSettings::~QxrdGlobalSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Application->printMessage("QxrdGlobalSettings::~QxrdGlobalSettings");
  }
}
