#include "qxrddebug.h"
#include <stdio.h>

QxrdDebugDictionary::QxrdDebugDictionary()
  : QcepDebugDictionary()
{
  setMessage(DEBUG_ACQUIRE,      "Acquisition Operations");
  setMessage(DEBUG_PROCESS,      "Processing Operations");
  setMessage(DEBUG_FILES,        "File Operations");
  setMessage(DEBUG_PERFORM,      "Performance Measurement");
  setMessage(DEBUG_IMAGES,       "Image Operations");
  setMessage(DEBUG_PERKINELMER,  "Perkin Elmer Acquisition");
  setMessage(DEBUG_PERKINELMERIDLING, "Perkin Elmer Idle Messages");
  setMessage(DEBUG_EXITWAIT,     "Halt at exit");
  setMessage(DEBUG_DELAY_ACQ,    "Delay Acquisition Startup");
  setMessage(DEBUG_BROWSER,      "File Browser Operations");
  setMessage(DEBUG_ACQUIRETIME,  "Acquisition Timing Messages");
  setMessage(DEBUG_INTEGRATOR,   "Integrator Operation");
  setMessage(DEBUG_TRIGGER,      "Acquisition Triggering");
  setMessage(DEBUG_EXTRAINPUTS,  "Extra Inputs");
  setMessage(DEBUG_LOGSCRIPTS,   "Log Script Commands");
  setMessage(DEBUG_PLUGINS,      "Plugin Loading");
  setMessage(DEBUG_FITTING,      "Curve Fitting");
  setMessage(DEBUG_NOPARALLEL,   "Serialize Various Algorithms");
  setMessage(DEBUG_HISTOGRAM,    "Debug Histogramming");
  setMessage(DEBUG_CALIBRANT,    "Powder Calibrant Calculations");
  setMessage(DEBUG_PILATUS,      "Pilatus Detector Acquisition");
  setMessage(DEBUG_ROI,          "Debug ROI Operations");
}
