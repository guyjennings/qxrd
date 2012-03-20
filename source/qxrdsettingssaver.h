#ifndef QXRDSETTINGSSAVER_H
#define QXRDSETTINGSSAVER_H

#include "qcepmacros.h"
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QDateTime>

#include "qcepproperty-ptr.h"

class QxrdSettingsSaver : public QObject
{
  Q_OBJECT
public:
  explicit QxrdSettingsSaver(QObject *owner);
  ~QxrdSettingsSaver();

  void start();
  void changed(QcepProperty *prop);

public slots:
  void performSave();
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

private:
  QMutex             m_Mutex;
  QObject           *m_Owner;
  QAtomicInt         m_ChangeCount;
  QTimer             m_Timer;
  int                m_SaveDelay;
  QcepProperty      *m_LastChangedBy;
};

#endif // QXRDSETTINGSSAVER_H
