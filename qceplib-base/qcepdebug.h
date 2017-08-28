#ifndef QCEPDEBUG_H
#define QCEPDEBUG_H

#include "qceplib_global.h"
#include <QStringList>
#include <QMap>
#include <QSharedPointer>

enum {
  DEBUG_NOMESSAGES         = 0,
  DEBUG_APP,
  DEBUG_PROPERTIES,
  DEBUG_DYNPROPS,
  DEBUG_WINDOW,
  DEBUG_PREFS,
  DEBUG_DISPLAY,
  DEBUG_CONSTRUCTORS,
  DEBUG_ALLOCATOR,
  DEBUG_THREADS,
  DEBUG_QUEUES,
  DEBUG_IMAGE_CONSTRUCTORS,
  DEBUG_LOCKING,
  DEBUG_SERVER,
  DEBUG_IMPORT,
  DEBUG_EXPORT,
  DEBUG_DATABROWSER,
  DEBUG_DRAGDROP,
  DEBUG_PANEL_SHOW,
  LAST_CEP_DEBUG           = DEBUG_PANEL_SHOW
};

class QCEP_EXPORT QcepDebugDictionary : public QObject {
public:
  QcepDebugDictionary();

  qint64 debugLevel() const;
  void setDebugLevel(qint64 level);

  QString message(int val) const;

  static QString hexArg(void *p);

protected:
  void setMessage(int val, QString msg);

private:
  qint64             m_DebugLevel;
  QMap<int, QString> m_Messages;
};

extern QCEP_EXPORT qint64 qcepDebug(int cond);

extern QCEP_EXPORT QSharedPointer<QcepDebugDictionary> g_DebugLevel;

#define HEXARG(a) arg(QcepDebugDictionary::hexArg(a))

#endif // QCEPDEBUG_H
