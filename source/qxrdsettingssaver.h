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
  Q_OBJECT;
public:
  explicit QxrdSettingsSaver(QObject *parent, QxrdApplication *app);

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime());

public slots:
  void performSave();

private:
  QMutex             m_Mutex;
  QxrdApplication   *m_Application;
  QAtomicInt         m_UpdateCount;
  QTimer             m_Timer;
  int                m_SaveDelay;
};

#endif // QXRDSETTINGSSAVER_H
