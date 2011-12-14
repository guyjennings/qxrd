#ifndef QXRDSETTINGSSAVER_H
#define QXRDSETTINGSSAVER_H

#include "qcepmacros.h"
#include "qcepproperty.h"
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

  void changed(QcepProperty *prop);

public slots:
  void performSave();

private:
  QMutex             m_Mutex;
  QObject           *m_Owner;
  QAtomicInt         m_ChangeCount;
  QTimer             m_Timer;
  int                m_SaveDelay;
  QcepProperty      *m_LastChangedBy;
};

#endif // QXRDSETTINGSSAVER_H
