#ifndef QXRDDEBUG_H
#define QXRDDEBUG_H

#include "qxrdlib_global.h"
#include "qcepdebug.h"

namespace QxrdDebug {
  Q_NAMESPACE

  enum QxrdDebugFlags {
    DEBUG_ACQUIRE     = LAST_QCEP_DEBUG,
    DEBUG_PROCESS,
    DEBUG_DELAY_ACQ,
    DEBUG_BROWSER,
    DEBUG_ACQUIRETIME,
    DEBUG_INTEGRATOR,
    DEBUG_TRIGGER,
    DEBUG_EXTRAINPUTS,
    DEBUG_LOGSCRIPTS,
    DEBUG_PLUGINS,
    DEBUG_HISTOGRAM,
    DEBUG_DETECTORIDLING,
    DEBUG_PERKINELMER,
    DEBUG_PILATUS,
    DEBUG_DEXELA,
    DEBUG_AREADETECTOR,
    DEBUG_SIMULATED,
    DEBUG_FILEWATCHER,
    DEBUG_NIDAQ,
    LAST_QXRD_DEBUG
  };

  Q_ENUM_NS(QxrdDebugFlags)

  class QXRD_EXPORT QxrdDebugDictionary : public QcepDebugDictionary {
    Q_OBJECT

  private:
    typedef QcepDebugDictionary inherited;

  public:
    QxrdDebugDictionary();

    int     debugFlagCount();
    virtual QString debugFlagName(int i);
    virtual int     debugFlagCheck(QString name);
  };
}

using namespace QxrdDebug;

#endif // QXRDDEBUG_H
