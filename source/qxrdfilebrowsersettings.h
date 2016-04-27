#ifndef QXRDFILEBROWSERSETTINGS_H
#define QXRDFILEBROWSERSETTINGS_H

#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"

class QxrdFileBrowserSettings : public QcepObject
{
  Q_OBJECT
public:
  explicit QxrdFileBrowserSettings(QcepObjectWPtr parent);
  
public:
  Q_PROPERTY(int browserFilter READ get_BrowserFilter WRITE set_BrowserFilter)
  QCEP_INTEGER_PROPERTY(BrowserFilter)

  Q_PROPERTY(QString browserSelector READ get_BrowserSelector WRITE set_BrowserSelector)
  QCEP_STRING_PROPERTY(BrowserSelector)

  Q_PROPERTY(QString rootDirectory READ get_RootDirectory WRITE set_RootDirectory)
  QCEP_STRING_PROPERTY(RootDirectory)

//  Q_PROPERTY(QcepIntList columnWidths READ get_ColumnWidths WRITE set_ColumnWidths)
//  QCEP_INTEGER_LIST_PROPERTY(ColumnWidths)

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

signals:
  
public slots:
  
};

typedef QSharedPointer<QxrdFileBrowserSettings> QxrdFileBrowserSettingsPtr;
typedef QWeakPointer<QxrdFileBrowserSettings>   QxrdFileBrowserSettingsWPtr;

#endif // QXRDFILEBROWSERSETTINGS_H
