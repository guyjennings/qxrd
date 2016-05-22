#include "qcepapplicationsettings.h"

QcepApplicationSettings *g_ApplicationSettings = NULL;

QcepApplicationSettings::QcepApplicationSettings(QcepApplicationWPtr app) :
  QcepSerializableObject("appSettings")
{
  g_ApplicationSettings = this;
}
