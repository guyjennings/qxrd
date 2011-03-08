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

extern const char* gDebugStrings[];

#define QCEP_DEBUG(cond,action) if (gCEPDebug & (cond)) { action; }

extern int qcepDebug(int cond);

#endif // QCEPDEBUG_H
