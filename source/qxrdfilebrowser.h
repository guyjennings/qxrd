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
#include "qxrdfilebrowsersettings.h"

class QxrdFileBrowser : public QDockWidget, public Ui::QxrdFileBrowser
{
  Q_OBJECT

public:
  QxrdFileBrowser(QxrdFileBrowserSettingsWPtr settings,
                  int isOutput,
                  QxrdExperimentWPtr experiment,
                  QxrdDataProcessorWPtr processor,
                  QWidget *parent);
  virtual ~QxrdFileBrowser();

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
  void doAdd();
  void doSubtract();
  void doProjectX();
  void doProjectY();
  void doProjectZ();
  void doCorrelate();
  void doEvaluate(QString filePath);
  void onRootDirectoryChanged(QString dir);
  void mousePressed(QModelIndex index);
  void onRowCountChanged(int oldCount, int newCount);
  void doubleClicked(QModelIndex index);
  void doSelectComboItem(int index);
  void onModelReset();
  void onFileUpdated(QFileInfo file);

private:
  QxrdFileBrowserSettingsWPtr  m_FileBrowserSettings;
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
  QxrdInputFileBrowser(QxrdFileBrowserSettingsWPtr settings, QxrdExperimentWPtr experiment, QxrdDataProcessorWPtr processor, QWidget *parent);
};

class QxrdOutputFileBrowser : public QxrdFileBrowser
{
  Q_OBJECT

public:
  QxrdOutputFileBrowser(QxrdFileBrowserSettingsWPtr settings, QxrdExperimentWPtr experiment, QxrdDataProcessorWPtr processor, QWidget *parent);
};

#endif // QXRDFILEBROWSER_H
