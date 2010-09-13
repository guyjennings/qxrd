/*******************************************************************
*
*     $Id: qcepdebug.h,v 1.2 2010/09/13 20:00:25 jennings Exp $
*
*******************************************************************/

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

#define QCEP_DEBUG(cond,action) if (gCEPDebug & (cond)) { action; }

#endif // QCEPDEBUG_H

/*******************************************************************
*
*     $Log: qcepdebug.h,v $
*     Revision 1.2  2010/09/13 20:00:25  jennings
*     Merged
*
*     Revision 1.1.2.5  2010/05/27 21:13:14  jennings
*     Added more debug flags
*     Tried to detect when processing is too slow for acquisition
*
*     Revision 1.1.2.4  2010/05/20 20:14:44  jennings
*     Added EXITWAIT debug option to stop app from quitting after QxrdApplication finishes
*
*     Revision 1.1.2.3  2010/05/19 20:26:12  jennings
*     Added DEBUG_PREFS debugging options
*
*     Revision 1.1.2.2  2010/05/17 21:06:32  jennings
*     Various PE related mods
*
*     Revision 1.1.2.1  2010/05/17 16:00:26  jennings
*     Debug printing macro
*
*
*******************************************************************/
