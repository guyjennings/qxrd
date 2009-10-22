/******************************************************************
*
*  $Id: qxrdfilebrowser.h,v 1.2 2009/10/22 21:50:58 jennings Exp $
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
  void onSelectorChanged(QString str);
  void doOpen();
  void doProcess();

private:
  mutable QMutex     m_Mutex;
  QxrdDataProcessor *m_Processor;
  QFileSystemModel  *m_Model;
  HEADER_IDENT("$Id: qxrdfilebrowser.h,v 1.2 2009/10/22 21:50:58 jennings Exp $");
};

#endif // QXRDFILEBROWSER_H

/******************************************************************
*
*  $Log: qxrdfilebrowser.h,v $
*  Revision 1.2  2009/10/22 21:50:58  jennings
*  More code for file browser, basic operation now works
*
*  Revision 1.1  2009/10/22 19:22:28  jennings
*  Initial file browser
*
*
*******************************************************************/
