#include "qxrddebug.h"
#include "qxrdglobalsettings.h"
#include <stdio.h>
#include "qxrdapplication.h"

QxrdGlobalSettings::QxrdGlobalSettings(QxrdApplication *app)
  : QSettings("cep.xray.aps.anl.gov","qxrd-global-4",NULL),
    m_Application(app)
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Application->printMessage("QxrdGlobalSettings::QxrdGlobalSettings");
  }
}

QxrdGlobalSettings::QxrdGlobalSettings(const QString &path, QSettings::Format format, QcepObject *parent)
  : QSettings(path, format, parent)
{

}

QxrdGlobalSettings::~QxrdGlobalSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Application->printMessage("QxrdGlobalSettings::~QxrdGlobalSettings");
  }
}
