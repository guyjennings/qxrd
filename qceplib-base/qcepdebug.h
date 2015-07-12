#ifndef QCEPDEBUG_H
#define QCEPDEBUG_H

#include <QStringList>
#include <QMap>
#include <QSharedPointer>

enum {
  DEBUG_NOMESSAGES         = 0,
  DEBUG_APP,
  DEBUG_PROPERTIES,
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
  DEBUG_DATABROWSER,
  DEBUG_DRAGDROP,
  LAST_CEP_DEBUG           = DEBUG_DRAGDROP
};

class QcepDebugDictionary : public QObject {
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

extern qint64 qcepDebug(int cond);

extern QSharedPointer<QcepDebugDictionary> g_DebugLevel;

#define HEXARG(a) arg(QcepDebugDictionary::hexArg(a))

#endif // QCEPDEBUG_H
