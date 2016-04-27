#include "qxrdfilebrowsersettings.h"
#include "qcepsettingssaver.h"

QxrdFileBrowserSettings::QxrdFileBrowserSettings(QcepObjectWPtr parent) :
  QcepObject("fileBrowserSettings", parent),
  m_BrowserFilter(this, "browserFilter",1, "Browser Filter"),
  m_BrowserSelector(this, "browserSelector","", "Browser Selector"),
  m_RootDirectory(this, "rootDirectory","", "Browser Root Directory")/*,
  m_ColumnWidths(this, "columnWidths", QcepIntList(), "Browser Column Widths")*/
{
}

void QxrdFileBrowserSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, settings, section);
}

void QxrdFileBrowserSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, settings, section);
}

