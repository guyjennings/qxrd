#include "qcepapplicationsettings.h"

QcepApplicationSettings *g_ApplicationSettings = NULL;

QcepApplicationSettings::QcepApplicationSettings(QcepApplicationWPtr app) :
  QcepObject("appSettings")
{
  g_ApplicationSettings = this;
}
