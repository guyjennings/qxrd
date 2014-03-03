#include "qcepdebug.h"
#include <stdio.h>

QSharedPointer<QcepDebugDictionary> g_DebugLevel;

QcepDebugDictionary::QcepDebugDictionary() :
  m_DebugLevel(0)
{
  setMessage(DEBUG_NOMESSAGES, "Disable All Debug Messages");
  setMessage(DEBUG_APP,        "Application startup and shutdown");
  setMessage(DEBUG_PROPERTIES, "Properties");
  setMessage(DEBUG_WINDOW,     "Window Operations");
  setMessage(DEBUG_PREFS,      "Settings/Prefs Operations");
  setMessage(DEBUG_DISPLAY,    "Display Operations");
  setMessage(DEBUG_CONSTRUCTORS, "Object Constructors/Destructors");
  setMessage(DEBUG_IMAGE_CONSTRUCTORS, "Image Object Constructors/Destructors");
  setMessage(DEBUG_LOCKING,    "Debug Mutex Lock Operations");
  setMessage(DEBUG_SERVER,       "Server Commands");
}

int qcepDebug(int cond)
{
  if (g_DebugLevel == NULL) {
    printf("No Debug Dictionary Allocated\n");

    return 0;
  } else {
    return g_DebugLevel->debugLevel() & cond;
  }
}

int QcepDebugDictionary::debugLevel() const
{
  return m_DebugLevel;
}

void QcepDebugDictionary::setDebugLevel(int level)
{
  m_DebugLevel = level;
}

QString QcepDebugDictionary::message(int val) const
{
  return m_Messages.value(val);
}

void QcepDebugDictionary::setMessage(int val, QString msg)
{
  if (m_Messages.contains(val)) {
    printf("Warning: duplicate messages for value %x\nExisting: %s\nNew:%s\n",
           val, qPrintable(message(val)), qPrintable(msg));
  }

  m_Messages.insert(val, msg);
}

QString QcepDebugDictionary::hexArg(void *p)
{
#if (QT_POINTER_SIZE==4)
  return tr("0x%1").arg((quint32)p, 8, 16, QLatin1Char('0'));
#else
  return tr("0x%1").arg((quint64)p, 16, 16, QLatin1Char('0'));
#endif
}

