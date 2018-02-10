#include "qxrdappviewersettings.h"
#include "qxrdappviewer.h"

QxrdAppViewerSettings::QxrdAppViewerSettings(QString name,
                                             int argc,
                                             char **argv)
  : inherited(name, argc, argv)
{
}

QxrdAppViewerSettings::~QxrdAppViewerSettings()
{
}

void QxrdAppViewerSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}
