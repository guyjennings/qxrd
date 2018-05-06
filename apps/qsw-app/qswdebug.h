#ifndef QSWDEBUG_H
#define QSWDEBUG_H

#include "qswlib_global.h"
#include "qcepdebug.h"

namespace QswDebug {
  Q_NAMESPACE

  enum QswDebugFlags {
    DEBUG_ACQUIRE   = LAST_QCEP_DEBUG,
    DEBUG_WINDOW,
    LAST_QSW_DEBUG
  };

  Q_ENUM_NS(QswDebugFlags)

  class /*QSW_EXPORT*/ QswDebugDictionary : public QcepDebugDictionary {
    Q_OBJECT

  private:
    typedef QcepDebugDictionary inherited;

  public:
    QswDebugDictionary();

    int     debugFlagCount();
    virtual QString debugFlagName(int i);
    virtual int     debugFlagCheck(QString name);
  };
}

using namespace QswDebug;

#endif // QSWDEBUG_H
