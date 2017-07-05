#ifndef QCEPAPPLICATIONSETTINGS_H
#define QCEPAPPLICATIONSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepapplication-ptr.h"

class QcepApplicationSettings : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepApplicationSettings(QcepApplicationWPtr app);
};

extern QcepApplicationSettings *g_ApplicationSettings;

#endif // QCEPAPPLICATIONSETTINGS_H
