#ifndef QXRDSETTINGSSAVER_H
#define QXRDSETTINGSSAVER_H

#include "qcepmacros.h"
#include <QObject>
#include <QTimer>
#include <QMutex>

#include "qxrdforwardtypes.h"

class QxrdSettingsSaver : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdSettingsSaver(QObject *parent, QxrdApplicationPtr app);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  void performSave();

private:
  QMutex             m_Mutex;
  QxrdApplicationPtr m_Application;
  QAtomicInt         m_UpdateCount;
  QTimer             m_Timer;
  int                m_SaveDelay;
};

#endif // QXRDSETTINGSSAVER_H
