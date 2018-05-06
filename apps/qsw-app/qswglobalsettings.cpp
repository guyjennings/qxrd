#include "qswglobalsettings.h"

QswGlobalSettings::QswGlobalSettings()
  : QSettings("cep.xray.aps.anl.gov", "qsw-global", NULL)
{
}
