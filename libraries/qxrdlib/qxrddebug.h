#ifndef QXRDDEBUG_H
#define QXRDDEBUG_H

#include "qxrdlib_global.h"
#include "qcepdebug.h"

namespace QxrdDebug {
  Q_NAMESPACE

  enum QxrdDebugFlags {
    DEBUG_ACQUIRE     = LAST_QCEP_DEBUG,
    DEBUG_PROCESS,
    DEBUG_FILES,
    DEBUG_PERFORM,
    DEBUG_IMAGES,
    DEBUG_EXITWAIT,
    DEBUG_DELAY_ACQ,
    DEBUG_BROWSER,
    DEBUG_ACQUIRETIME,
    DEBUG_INTEGRATOR,
    DEBUG_TRIGGER,
    DEBUG_EXTRAINPUTS,
    DEBUG_LOGSCRIPTS,
    DEBUG_PLUGINS,
    DEBUG_FITTING,
    DEBUG_NOPARALLEL,
    DEBUG_HISTOGRAM,
    DEBUG_CALIBRANT,
    DEBUG_DETECTORIDLING,
    DEBUG_PERKINELMER,
    DEBUG_PILATUS,
    DEBUG_DEXELA,
    DEBUG_AREADETECTOR,
    DEBUG_SIMULATED,
    DEBUG_FILEWATCHER,
    DEBUG_ROI,
    LAST_QXRD_DEBUG
  };

  Q_ENUM_NS(QxrdDebugFlags)

  class QXRD_EXPORT QxrdDebugDictionary : public QcepDebugDictionary {
    Q_OBJECT

  public:
    QxrdDebugDictionary();
  };
}

using namespace QxrdDebug;

#endif // QXRDDEBUG_H
