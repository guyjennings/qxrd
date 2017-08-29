#ifndef QXRDFILEBROWSERSETTINGSPTR_H
#define QXRDFILEBROWSERSETTINGSPTR_H

#include <QSharedPointer>

class QxrdFileBrowserSettings;

typedef QSharedPointer<QxrdFileBrowserSettings> QxrdFileBrowserSettingsPtr;
typedef QWeakPointer<QxrdFileBrowserSettings>   QxrdFileBrowserSettingsWPtr;

class QxrdInputFileBrowserSettings;

typedef QSharedPointer<QxrdInputFileBrowserSettings> QxrdInputFileBrowserSettingsPtr;
typedef QWeakPointer<QxrdInputFileBrowserSettings>   QxrdInputFileBrowserSettingsWPtr;

class QxrdOutputFileBrowserSettings;

typedef QSharedPointer<QxrdOutputFileBrowserSettings> QxrdOutputFileBrowserSettingsPtr;
typedef QWeakPointer<QxrdOutputFileBrowserSettings>   QxrdOutputFileBrowserSettingsWPtr;

#endif // QXRDFILEBROWSERSETTINGSPTR_H
