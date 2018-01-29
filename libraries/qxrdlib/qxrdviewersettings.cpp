#include "qxrddebug.h"
#include "qxrdviewersettings.h"
#include <stdio.h>
#include "qxrdappcommon.h"

QxrdViewerSettings::QxrdViewerSettings(QxrdAppCommon *app)
  : QSettings("cep.xray.aps.anl.gov","qxrdviewer-global-1",NULL),
    m_Application(app)
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Application->printMessage("QxrdViewerSettings::QxrdViewerSettings");
  }
}

QxrdViewerSettings::QxrdViewerSettings(const QString &path, QSettings::Format format, QcepObject *parent)
  : QSettings(path, format, parent)
{

}

QxrdViewerSettings::~QxrdViewerSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Application->printMessage("QxrdViewerSettings::~QxrdViewerSettings");
  }
}
