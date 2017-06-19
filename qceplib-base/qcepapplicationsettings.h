#ifndef QCEPAPPLICATIONSETTINGS_H
#define QCEPAPPLICATIONSETTINGS_H

#include "qcepobject.h"
#include "qcepapplication-ptr.h"

class QcepApplicationSettings : public QcepObject
{
  Q_OBJECT

public:
  QcepApplicationSettings(QcepApplicationWPtr app);
};

extern QcepApplicationSettings *g_ApplicationSettings;

#endif // QCEPAPPLICATIONSETTINGS_H
