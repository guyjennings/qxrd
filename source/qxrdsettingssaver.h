#ifndef QXRDSETTINGSSAVER_H
#define QXRDSETTINGSSAVER_H

#include "qcepmacros.h"
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QDateTime>

#include "qcepproperty-ptr.h"
#include "qcepsettingssaver.h"

class QxrdSettingsSaver : public QcepSettingsSaver
{
  Q_OBJECT
public:
  explicit QxrdSettingsSaver(QObject *owner);
  ~QxrdSettingsSaver();
};

#endif // QXRDSETTINGSSAVER_H
