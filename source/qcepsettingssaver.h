#ifndef QCEPSETTINGSSAVER_H
#define QCEPSETTINGSSAVER_H

#include "qcepmacros.h"
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QDateTime>

#include "qcepproperty-ptr.h"

class QcepSettingsSaver : public QObject
{
  Q_OBJECT
public:
  explicit QcepSettingsSaver(QObject *owner);
  ~QcepSettingsSaver();

  void changed(QcepProperty *prop = NULL);

public slots:
  void start();
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

#endif // QCEPSETTINGSSAVER_H
