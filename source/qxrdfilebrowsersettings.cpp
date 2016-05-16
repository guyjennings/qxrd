#include "qxrdfilebrowsersettings.h"
#include "qcepsettingssaver.h"

QxrdFileBrowserSettings::QxrdFileBrowserSettings(QString name) :
  QcepObject(name),
  m_BrowserFilter(this, "browserFilter",1, "Browser Filter"),
  m_BrowserSelector(this, "browserSelector","", "Browser Selector"),
  m_RootDirectory(this, "rootDirectory","", "Browser Root Directory")
{
}

