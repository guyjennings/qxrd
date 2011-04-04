#ifndef QXRDFILEBROWSER_H
#define QXRDFILEBROWSER_H

#include "qcepmacros.h"

#include <QDockWidget>
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "ui_qxrdfilebrowser.h"

class QxrdFileBrowserModelThread;
class QxrdFileBrowserModel;
class QxrdDataProcessor;

class QxrdFileBrowser : public QDockWidget, public Ui::QxrdFileBrowser
{
  Q_OBJECT;

public:
  QxrdFileBrowser(int isOutput, QxrdDataProcessor *processor, QWidget *parent=0);

public:
  Q_PROPERTY(int browserFilter READ get_BrowserFilter WRITE set_BrowserFilter);
  QCEP_INTEGER_PROPERTY(BrowserFilter);

  Q_PROPERTY(QString browserSelector READ get_BrowserSelector WRITE set_BrowserSelector);
  QCEP_STRING_PROPERTY(BrowserSelector);

  Q_PROPERTY(QString rootDirectory READ get_RootDirectory WRITE set_RootDirectory);
  QCEP_STRING_PROPERTY(RootDirectory);

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

public slots:
  void onFilterChanged(int newfilter);
  void onSelectorChanged(QString str, const QModelIndex &a= QModelIndex());
  void doPushDirectory(QString newDir);
  void doPreviousDirectory();
  void doUpDirectory();
  void doChangeDirectory();
  void doHomeDirectory();
  void doAcquisitionDirectory();
  void doRefreshBrowser();
  void doOpen();
  void doProcess();
  void doIntegrate();
  void doAccumulate();
  void onRootDirectoryChanged(QString dir);
  void mousePressed(QModelIndex index);
  void onRowCountChanged(int oldCount, int newCount);
  void doubleClicked(QModelIndex index);
  void doSelectComboItem(int index);
  void onModelReset();

signals:
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;
  void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

private:
  mutable QMutex               m_Mutex;
  int                          m_IsOutput;
  QxrdDataProcessor           *m_Processor;
  QxrdFileBrowserModelThread  *m_ModelThread;
  QxrdFileBrowserModel        *m_Model;
  QStringList                  m_DirectoryStack;
};

class QxrdInputFileBrowser : public QxrdFileBrowser
{
  Q_OBJECT;

public:
  QxrdInputFileBrowser(QxrdDataProcessor *processor, QWidget *parent=0);
};

class QxrdOutputFileBrowser : public QxrdFileBrowser
{
  Q_OBJECT;

public:
  QxrdOutputFileBrowser(QxrdDataProcessor *processor, QWidget *parent=0);
};

#endif // QXRDFILEBROWSER_H
