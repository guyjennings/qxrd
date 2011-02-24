#ifndef QCEPDEBUG_H
#define QCEPDEBUG_H

extern int gCEPDebug;

enum {
  DEBUG_APP = 1,
  DEBUG_ACQUIRE = 2,
  DEBUG_PROCESS = 4,
  DEBUG_DISPLAY = 8,
  DEBUG_FILES = 16,
  DEBUG_QUEUES = 32,
  DEBUG_PERFORM = 64,
  DEBUG_IMAGES = 128,
  DEBUG_PROPERTIES = 256,
  DEBUG_WINDOW = 512,
  DEBUG_PERKINELMER = 1024,
  DEBUG_PREFS = 2048,
  DEBUG_EXITWAIT = 4096,
  DEBUG_ALLOCATOR = 8192
};

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
  "Memory Allocation"
};

#define QCEP_DEBUG(cond,action) if (gCEPDebug & (cond)) { action; }

#endif // QCEPDEBUG_H
