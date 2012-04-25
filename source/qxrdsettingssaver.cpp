#include "qxrddebug.h"
#include "qcepmacros.h"
#include "qxrdsettingssaver.h"
#include "qcepproperty.h"
#include "qxrdapplication.h"
#include <stdio.h>
#include "qxrdmutexlocker.h"

QxrdSettingsSaver::QxrdSettingsSaver(QObject *owner) :
  QcepSettingsSaver(owner)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSettingsSaver::QxrdSettingsSaver(%p)\n", this);
  }
}

QxrdSettingsSaver::~QxrdSettingsSaver()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdSettingsSaver::~QxrdSettingsSaver(%p)\n", this);
  }
}
