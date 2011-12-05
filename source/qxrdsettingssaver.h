#ifndef QXRDSETTINGSSAVER_H
#define QXRDSETTINGSSAVER_H

#include "qcepmacros.h"
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QDateTime>

class QxrdApplication;

class QxrdSettingsSaver : public QObject
{
  Q_OBJECT
public:
  explicit QxrdSettingsSaver(QObject *parent, QObject *owner);
  ~QxrdSettingsSaver();

  void changed();

public slots:
  void performSave();

private:
  QMutex             m_Mutex;
  QObject           *m_Owner;
  QAtomicInt         m_ChangeCount;
  QTimer             m_Timer;
  int                m_SaveDelay;
};

#endif // QXRDSETTINGSSAVER_H
