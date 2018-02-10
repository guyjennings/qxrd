#include "qcepapplicationsettings.h"

QcepApplicationSettings *g_ApplicationSettings = NULL;

QcepApplicationSettings::QcepApplicationSettings(QString name) :
  inherited(name)
{
  g_ApplicationSettings = this;
}

void QcepApplicationSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}
