#include "qxrddebug.h"
#include <stdio.h>

QxrdDebugDictionary::QxrdDebugDictionary()
  : inherited()
{
  setMessage(DEBUG_ACQUIRE,      "Acquisition Operations");
  setMessage(DEBUG_PROCESS,      "Processing Operations");
  setMessage(DEBUG_DELAY_ACQ,    "Delay Acquisition Startup");
  setMessage(DEBUG_BROWSER,      "File Browser Operations");
  setMessage(DEBUG_ACQUIRETIME,  "Acquisition Timing Messages");
  setMessage(DEBUG_INTEGRATOR,   "Integrator Operation");
  setMessage(DEBUG_TRIGGER,      "Acquisition Triggering");
  setMessage(DEBUG_EXTRAINPUTS,  "Extra Inputs");
  setMessage(DEBUG_LOGSCRIPTS,   "Log Script Commands");
  setMessage(DEBUG_PLUGINS,      "Plugin Loading");
  setMessage(DEBUG_HISTOGRAM,    "Debug Histogramming");
  setMessage(DEBUG_DETECTORIDLING, "Perkin Elmer Idle Messages");
  setMessage(DEBUG_PERKINELMER,  "Perkin Elmer Acquisition");
  setMessage(DEBUG_PILATUS,      "Pilatus Detector Acquisition");
  setMessage(DEBUG_DEXELA,       "Dexela Detector Acquisition");
  setMessage(DEBUG_AREADETECTOR, "Area Detector Acquisition");
  setMessage(DEBUG_SIMULATED,    "Simulated Detector Acquisition");
  setMessage(DEBUG_FILEWATCHER,  "File Watcher Acquisition");
  setMessage(DEBUG_NIDAQ,        "NIDAQ Operations");
}

int QxrdDebugDictionary::debugFlagCount()
{
  return LAST_QXRD_DEBUG;
}

QString QxrdDebugDictionary::debugFlagName(int i)
{
  if (i < inherited::debugFlagCount()) {
    return QcepDebugDictionary::debugFlagName(i);
  } else {
    return debugFlagNameFrom(&QxrdDebug::staticMetaObject,
                             i-inherited::debugFlagCount());
  }
}

int QxrdDebugDictionary::debugFlagCheck(QString name)
{
  int res = debugFlagCheckFrom(&QxrdDebug::staticMetaObject,
                               name);

  if (res < 0) {
    res = inherited::debugFlagCheck(name);
  }

  return res;
}
