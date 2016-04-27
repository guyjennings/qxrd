#include "qcepapplicationsettings.h"

QcepApplicationSettings *g_ApplicationSettings = NULL;

QcepApplicationSettings::QcepApplicationSettings(QcepApplicationWPtr app) :
  QcepObject("appSettings", QcepObjectWPtr())
{
  g_ApplicationSettings = this;
}
