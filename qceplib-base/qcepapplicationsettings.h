#ifndef QCEPAPPLICATIONSETTINGS_H
#define QCEPAPPLICATIONSETTINGS_H

#include "qceplib_global.h"
#include "qcepserializableobject.h"
#include "qcepapplication-ptr.h"

class QCEP_EXPORT QcepApplicationSettings : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepApplicationSettings(QcepApplicationWPtr app);
};

extern QcepApplicationSettings *g_ApplicationSettings;

#endif // QCEPAPPLICATIONSETTINGS_H
