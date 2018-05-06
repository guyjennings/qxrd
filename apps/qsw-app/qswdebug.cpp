#include "qswdebug.h"

QswDebugDictionary::QswDebugDictionary()
  : QcepDebugDictionary()
{
  setMessage(DEBUG_ACQUIRE,        "Acquisition Operations");
  setMessage(DEBUG_WINDOW,         "Window Operations");
}

int QswDebugDictionary::debugFlagCount()
{
  return LAST_QSW_DEBUG;
}

QString QswDebugDictionary::debugFlagName(int i)
{
  if (i < inherited::debugFlagCount()) {
    return QcepDebugDictionary::debugFlagName(i);
  } else {
    return debugFlagNameFrom(&QswDebug::staticMetaObject,
                             i-inherited::debugFlagCount());
  }
}

int QswDebugDictionary::debugFlagCheck(QString name)
{
  int res = debugFlagCheckFrom(&QswDebug::staticMetaObject,
                               name);

  if (res < 0) {
    res = inherited::debugFlagCheck(name);
  }

  return res;
}
