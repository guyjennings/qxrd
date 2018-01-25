#include "qxrdappviewersettings.h"
#include "qxrdappviewer.h"

QxrdAppViewerSettings::QxrdAppViewerSettings(QxrdAppViewerWPtr app, int argc, char **argv)
  : inherited(app, argc, argv)
{

}

QxrdAppViewerSettings::~QxrdAppViewerSettings()
{
}

void QxrdAppViewerSettings::init()
{
  inherited::init();
}
