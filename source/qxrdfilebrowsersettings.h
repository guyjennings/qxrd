#ifndef QXRDFILEBROWSERSETTINGS_H
#define QXRDFILEBROWSERSETTINGS_H

#include "qcepserializableobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdFileBrowserSettings : public QcepSerializableObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdFileBrowserSettings(QString name);
  QxrdFileBrowserSettings();

public:
  Q_PROPERTY(int browserFilter READ get_BrowserFilter WRITE set_BrowserFilter)
  QCEP_INTEGER_PROPERTY(BrowserFilter)

  Q_PROPERTY(QString browserSelector READ get_BrowserSelector WRITE set_BrowserSelector)
  QCEP_STRING_PROPERTY(BrowserSelector)

  Q_PROPERTY(QString rootDirectory READ get_RootDirectory WRITE set_RootDirectory)
  QCEP_STRING_PROPERTY(RootDirectory)
};

Q_DECLARE_METATYPE(QxrdFileBrowserSettings*)

class QxrdInputFileBrowserSettings : public QxrdFileBrowserSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdInputFileBrowserSettings(QString name);
  QxrdInputFileBrowserSettings();
};

Q_DECLARE_METATYPE(QxrdInputFileBrowserSettings*)

class QxrdOutputFileBrowserSettings : public QxrdFileBrowserSettings
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdOutputFileBrowserSettings(QString name);
  QxrdOutputFileBrowserSettings();
};

Q_DECLARE_METATYPE(QxrdOutputFileBrowserSettings*)

#endif // QXRDFILEBROWSERSETTINGS_H
