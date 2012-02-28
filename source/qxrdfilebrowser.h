#ifndef QXRDFILEBROWSER_H
#define QXRDFILEBROWSER_H

#include "qcepmacros.h"

#include <QDockWidget>
#include <QFileInfo>
#include "qcepproperty.h"
#include "ui_qxrdfilebrowser.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdfilebrowsermodel-ptr.h"
#include "qxrdexperiment-ptr.h"

class QxrdFileBrowser : public QDockWidget, public Ui::QxrdFileBrowser
{
  Q_OBJECT

public:
  QxrdFileBrowser(QxrdSettingsSaverPtr saver,
                  int isOutput,
                  QxrdExperimentWPtr experiment,
                  QxrdDataProcessorWPtr processor,
                  QWidget *parent=0);

public:
  Q_PROPERTY(int browserFilter READ get_BrowserFilter WRITE set_BrowserFilter)
  QCEP_INTEGER_PROPERTY(BrowserFilter)

  Q_PROPERTY(QString browserSelector READ get_BrowserSelector WRITE set_BrowserSelector)
  QCEP_STRING_PROPERTY(BrowserSelector)

  Q_PROPERTY(QString rootDirectory READ get_RootDirectory WRITE set_RootDirectory)
  QCEP_STRING_PROPERTY(RootDirectory)

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

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
  void doOpenDark();
  void doOpenMask();
  void doOpenGainMap();
  void doProcess();
  void doIntegrate();
  void doAccumulate();
  void onRootDirectoryChanged(QString dir);
  void mousePressed(QModelIndex index);
  void onRowCountChanged(int oldCount, int newCount);
  void doubleClicked(QModelIndex index);
  void doSelectComboItem(int index);
  void onModelReset();
  void onFileUpdated(QFileInfo file);

private:
  mutable QMutex               m_Mutex;
  int                          m_IsOutput;
  QxrdExperimentWPtr           m_Experiment;
  QxrdDataProcessorWPtr        m_Processor;
  QxrdFileBrowserModelPtr      m_Model;
  QStringList                  m_DirectoryStack;
};

class QxrdInputFileBrowser : public QxrdFileBrowser
{
  Q_OBJECT

public:
  QxrdInputFileBrowser(QxrdSettingsSaverPtr saver, QxrdExperimentWPtr experiment, QxrdDataProcessorWPtr processor, QWidget *parent);
};

class QxrdOutputFileBrowser : public QxrdFileBrowser
{
  Q_OBJECT

public:
  QxrdOutputFileBrowser(QxrdSettingsSaverPtr saver, QxrdExperimentWPtr experiment, QxrdDataProcessorWPtr processor, QWidget *parent);
};

#endif // QXRDFILEBROWSER_H
