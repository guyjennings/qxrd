#ifndef QXRDDEBUG_H
#define QXRDDEBUG_H

#include "qcepdebug.h"

enum {
  DEBUG_ACQUIRE     = (LAST_CEP_DEBUG << 1),
  DEBUG_PROCESS     = (DEBUG_ACQUIRE<<1),
  DEBUG_FILES       = (DEBUG_PROCESS<<1),
  DEBUG_QUEUES      = (DEBUG_FILES<<1),
  DEBUG_PERFORM     = (DEBUG_QUEUES<<1),
  DEBUG_IMAGES      = (DEBUG_PERFORM<<1),
  DEBUG_PERKINELMER = (DEBUG_IMAGES<<1),
  DEBUG_EXITWAIT    = (DEBUG_PERKINELMER<<1),
  DEBUG_ALLOCATOR   = (DEBUG_EXITWAIT<<1),
  DEBUG_THREADS     = (DEBUG_ALLOCATOR<<1),
  DEBUG_DELAY_ACQ   = (DEBUG_THREADS<<1),
  DEBUG_BROWSER     = (DEBUG_DELAY_ACQ<<1),
  DEBUG_SERVER      = (DEBUG_BROWSER<<1),
  DEBUG_ACQUIRETIME = (DEBUG_SERVER<<1),
  DEBUG_INTEGRATOR  = (DEBUG_ACQUIRETIME<<1),
  DEBUG_CONSTRUCTORS= (DEBUG_INTEGRATOR<<1),
  DEBUG_TRIGGER     = (DEBUG_CONSTRUCTORS<<1),
  DEBUG_EXTRAINPUTS = (DEBUG_TRIGGER<<1)
};

class QxrdDebugDictionary : public QcepDebugDictionary {
public:
  QxrdDebugDictionary();
};

#endif // QXRDDEBUG_H
