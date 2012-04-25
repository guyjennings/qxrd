#include "qxrdfilebrowsersettings.h"
#include "qxrdsettingssaver.h"

QxrdFileBrowserSettings::QxrdFileBrowserSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_BrowserFilter(saver, this, "browserFilter",1, "Browser Filter"),
  m_BrowserSelector(saver, this, "browserSelector","", "Browser Selector"),
  m_RootDirectory(saver, this, "rootDirectory","", "Browser Root Directory")/*,
  m_ColumnWidths(saver, this, "columnWidths", QcepIntList(), "Browser Column Widths")*/
{
}

void QxrdFileBrowserSettings::writeSettings(QSettings *settings, QString section)
{
  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdFileBrowserSettings::readSettings(QSettings *settings, QString section)
{
  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

