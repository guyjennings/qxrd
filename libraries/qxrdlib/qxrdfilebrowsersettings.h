#ifndef QXRDFILEBROWSERSETTINGS_H
#define QXRDFILEBROWSERSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qxrdfilebrowsersettings-ptr.h"

class QXRD_EXPORT QxrdFileBrowserSettings : public QcepObject
{
  Q_OBJECT
public:
  Q_INVOKABLE QxrdFileBrowserSettings(QString name);

public:
  Q_PROPERTY(int browserFilter READ get_BrowserFilter WRITE set_BrowserFilter)
  QCEP_INTEGER_PROPERTY(BrowserFilter)

  Q_PROPERTY(QString browserSelector READ get_BrowserSelector WRITE set_BrowserSelector)
  QCEP_STRING_PROPERTY(BrowserSelector)

  Q_PROPERTY(QString rootDirectory READ get_RootDirectory WRITE set_RootDirectory)
  QCEP_STRING_PROPERTY(RootDirectory)
};

Q_DECLARE_METATYPE(QxrdFileBrowserSettings*)

#endif // QXRDFILEBROWSERSETTINGS_H
