#ifndef QCEPDEBUG_H
#define QCEPDEBUG_H

#include <QStringList>
#include <QMap>
#include <QSharedPointer>

enum {
  DEBUG_NOMESSAGES         = 1,
  DEBUG_APP                = (DEBUG_NOMESSAGES << 1),
  DEBUG_PROPERTIES         = (DEBUG_APP << 1),
  DEBUG_WINDOW             = (DEBUG_PROPERTIES << 1),
  DEBUG_PREFS              = (DEBUG_WINDOW << 1),
  DEBUG_DISPLAY            = (DEBUG_PREFS << 1),
  DEBUG_CONSTRUCTORS       = (DEBUG_DISPLAY<<1),
  DEBUG_IMAGE_CONSTRUCTORS = (DEBUG_CONSTRUCTORS<<1),
  DEBUG_LOCKING            = (DEBUG_IMAGE_CONSTRUCTORS<<1),
  LAST_CEP_DEBUG           = DEBUG_LOCKING
};

class QcepDebugDictionary : public QObject {
public:
  QcepDebugDictionary();

  int debugLevel() const;
  void setDebugLevel(int level);

  QString message(int val) const;

  static QString hexArg(void *p);

protected:
  void setMessage(int val, QString msg);

private:
  int                m_DebugLevel;
  QMap<int, QString> m_Messages;
};

extern int qcepDebug(int cond);

extern QSharedPointer<QcepDebugDictionary> g_DebugLevel;

#define HEXARG(a) arg(QcepDebugDictionary::hexArg(a))

#endif // QCEPDEBUG_H
