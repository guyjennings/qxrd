#include "qcepdebug.h"
#include <stdio.h>

const char* gDebugStrings[] = {
  "Application startup and shutdown",
  "Acquisition Operations",
  "Processing Operations",
  "Display Operations",
  "File Operations",
  "Queue Operations",
  "Performance Measurement",
  "Image Operations",
  "Properties",
  "Window Operations",
  "Perkin Elmer Acquisition",
  "Settings/Prefs Operations",
  "Halt at exit",
  "Memory Allocation",
  "Thread Initialization",
  "Delay Acquisition Startup",
  "File Browser Operations",
  "Server Commands",
  "Disable Messages",
  "Acquisition Timing Messages",
  "Integrator Operation",
  "Object Constructors/Destructors",
  "Acquisition Triggering",
  NULL
};

int qcepDebug(int cond)
{
  return (gCEPDebug & (cond));
}
