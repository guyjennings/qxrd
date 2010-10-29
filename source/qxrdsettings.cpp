#include "qxrdsettings.h"
#include <stdio.h>

QxrdSettings::QxrdSettings(QObject *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd-2",parent)
{
  QCEP_DEBUG(DEBUG_PREFS,
             printf("QxrdSettings::QxrdSettings\n");
  )
}

QxrdSettings::~QxrdSettings()
{
  QCEP_DEBUG(DEBUG_PREFS,
             printf("QxrdSettings::~QxrdSettings\n");
  )
}
