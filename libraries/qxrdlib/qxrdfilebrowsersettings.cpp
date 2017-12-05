#include "qxrdfilebrowsersettings.h"

QxrdFileBrowserSettings::QxrdFileBrowserSettings(QString name) :
  QcepObject(name),
  m_BrowserFilter(this, "browserFilter",1, "Browser Filter"),
  m_BrowserSelector(this, "browserSelector","", "Browser Selector"),
  m_RootDirectory(this, "rootDirectory","", "Browser Root Directory")
{
}

QxrdFileBrowserSettingsPtr QxrdFileBrowserSettings::newFileBrowserSettings()
{
  QxrdFileBrowserSettingsPtr set(new QxrdFileBrowserSettings("fileBrowserSettings"));

  return set;
}

QxrdInputFileBrowserSettings::QxrdInputFileBrowserSettings(QString name) :
  QxrdFileBrowserSettings(name)
{
}

QxrdInputFileBrowserSettingsPtr QxrdInputFileBrowserSettings::newInputFileBrowserSettings()
{
  QxrdInputFileBrowserSettingsPtr set(new QxrdInputFileBrowserSettings("inputFileBrowserSettings"));

  return set;
}

QxrdOutputFileBrowserSettings::QxrdOutputFileBrowserSettings(QString name) :
  QxrdFileBrowserSettings(name)
{
}

QxrdOutputFileBrowserSettingsPtr QxrdOutputFileBrowserSettings::newOutputFileBrowserSettings()
{
  QxrdOutputFileBrowserSettingsPtr set(new QxrdOutputFileBrowserSettings("outputFileBrowserSettings"));

  return set;
}
