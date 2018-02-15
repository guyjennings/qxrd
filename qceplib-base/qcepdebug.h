#ifndef QCEPDEBUG_H
#define QCEPDEBUG_H

#include "qceplib_global.h"
#include <QStringList>
#include <QMap>
#include <QSharedPointer>

namespace QcepDebug {
  Q_NAMESPACE

  enum QcepDebugFlags {
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
    DEBUG_OBJECTBROWSER,
    DEBUG_DRAGDROP,
    DEBUG_PANEL_SHOW,
    LAST_QCEP_DEBUG
  };

  Q_ENUM_NS(QcepDebugFlags)

  class QCEP_EXPORT QcepDebugDictionary : public QObject {
    Q_OBJECT

  private:
    typedef QObject inherited;

  public:
    QcepDebugDictionary();

    qint64 debugLevel() const;
    void setDebugLevel(qint64 level);

    QString message(int val) const;

  protected:
    void setMessage(int val, QString msg);

    static int     debugFlagCount();

    static QString debugFlagName(int i);
    static QString debugFlagNameFrom(const QMetaObject &meta, int i);

    static int     debugFlagCheck(QString name);
    static int     debugFlagCheckFrom(const QMetaObject &meta, QString f);

  private:
    qint64             m_DebugLevel;
    QMap<int, QString> m_Messages;
  };
}

using namespace QcepDebug;

extern QCEP_EXPORT qint64 qcepDebug(int cond);

extern QCEP_EXPORT QSharedPointer<QcepDebugDictionary> g_DebugLevel;

#endif // QCEPDEBUG_H
