#ifndef QCEPAPPLICATIONSETTINGS_H
#define QCEPAPPLICATIONSETTINGS_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include "qcepapplication-ptr.h"

class QCEP_EXPORT QcepApplicationSettings : public QcepObject
{
  Q_OBJECT

public:
  QcepApplicationSettings(QcepApplicationWPtr app);
};

extern QCEP_EXPORT QcepApplicationSettings *g_ApplicationSettings;

#endif // QCEPAPPLICATIONSETTINGS_H
