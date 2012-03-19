#include "qxrdfilebrowsersettings.h"

QxrdFileBrowserSettings::QxrdFileBrowserSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent),
  m_BrowserFilter(saver, this, "browserFilter",1, "Browser Filter"),
  m_BrowserSelector(saver, this, "browserSelector","", "Browser Selector"),
  m_RootDirectory(saver, this, "rootDirectory","", "Browser Root Directory")
{
}

void QxrdFileBrowserSettings::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    if (m_FileBrowser) {
      settings->beginWriteArray(section+"/colWidths",4);

      for (int i=0; i<3; i++) {
        settings->setArrayIndex(i);
        settings->setValue("width", m_FileBrowser->columnWidth(i));
      }

      settings->endArray();
    }
  }
}

void QxrdFileBrowserSettings::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    int sz = settings->beginReadArray(section+"/colWidths");

    for (int i=0; i<sz; i++) {
      settings->setArrayIndex(i);
      int width = settings->value("width", -1).toInt();

      if (m_FileBrowser) {
        if (width > 5) {
          m_FileBrowser->setColumnWidth(i, width);
        } else {
          m_FileBrowser->setColumnWidth(i, 5);
        }
      }
    }

    settings->endArray();
  }
}

