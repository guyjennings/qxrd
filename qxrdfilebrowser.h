/******************************************************************
*
*  $Id: qxrdfilebrowser.h,v 1.4 2009/10/26 02:39:12 jennings Exp $
*
*******************************************************************/

#ifndef QXRDFILEBROWSER_H
#define QXRDFILEBROWSER_H

#include "qcepmacros.h"

class QxrdDataProcessor;
class QFileSystemModel;

#include <QWidget>
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "ui_qxrdfilebrowser.h"

class QxrdFileBrowser : public QWidget, public Ui::QxrdFileBrowser
{
  Q_OBJECT;

public:
  QxrdFileBrowser(QxrdDataProcessor *processor, QWidget *parent=0);

public:
  Q_PROPERTY(int browserFilter READ get_BrowserFilter WRITE set_BrowserFilter);
  QCEP_INTEGER_PROPERTY(BrowserFilter);

  Q_PROPERTY(QString browserSelector READ get_BrowserSelector WRITE set_BrowserSelector);
  QCEP_STRING_PROPERTY(BrowserSelector);

public:
  void readSettings(QxrdSettings *settings, QString section);
  void writeSettings(QxrdSettings *settings, QString section);

public slots:
  void onFilterChanged(int newfilter);
  void onSelectorChanged(QString str, QModelIndex = QModelIndex());
  void doOpen();
  void doProcess();
  void onRootDirectoryChanged(QString dir);
  void mousePressed(QModelIndex index);

signals:
  void printMessage(QString msg) const;
  void statusMessage(QString msg) const;
  void criticalMessage(QString msg) const;

private:
  mutable QMutex     m_Mutex;
  QxrdDataProcessor *m_Processor;
  QFileSystemModel  *m_Model;
  HEADER_IDENT("$Id: qxrdfilebrowser.h,v 1.4 2009/10/26 02:39:12 jennings Exp $");
};

#endif // QXRDFILEBROWSER_H

/******************************************************************
*
*  $Log: qxrdfilebrowser.h,v $
*  Revision 1.4  2009/10/26 02:39:12  jennings
*  Added right click menu routine for file browser
*
*  Revision 1.3  2009/10/23 19:42:01  jennings
*  Implement file selector box for file browser, change file browser root when output directory is changed
*
*  Revision 1.2  2009/10/22 21:50:58  jennings
*  More code for file browser, basic operation now works
*
*  Revision 1.1  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*
*******************************************************************/
