#include "qxrdfilebrowsersettings.h"
#include "qcepsettingssaver.h"

QxrdFileBrowserSettings::QxrdFileBrowserSettings(QString name) :
  QcepSerializableObject(name),
  m_BrowserFilter(this, "browserFilter",1, "Browser Filter"),
  m_BrowserSelector(this, "browserSelector","", "Browser Selector"),
  m_RootDirectory(this, "rootDirectory","", "Browser Root Directory")
{
}

QxrdFileBrowserSettings::QxrdFileBrowserSettings() :
  QxrdFileBrowserSettings("fileBrowserSettings")
{
}

QxrdInputFileBrowserSettings::QxrdInputFileBrowserSettings(QString name) :
  QxrdFileBrowserSettings(name)
{
}

QxrdInputFileBrowserSettings::QxrdInputFileBrowserSettings() :
  QxrdInputFileBrowserSettings("inputFileBrowserSettings")
{
}

QxrdOutputFileBrowserSettings::QxrdOutputFileBrowserSettings(QString name) :
  QxrdFileBrowserSettings(name)
{
}

QxrdOutputFileBrowserSettings::QxrdOutputFileBrowserSettings() :
  QxrdOutputFileBrowserSettings("outputFileBrowserSettings")
{
}
