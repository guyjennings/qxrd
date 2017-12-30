#ifndef QXRDFILEBROWSERWIDGET_H
#define QXRDFILEBROWSERWIDGET_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <QWidget>
#include "ui_qxrdfilebrowserwidget.h"
#include "qxrdfilebrowsersettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qxrdfilebrowsermodel-ptr.h"
#include <QFileInfo>
#include <QMutex>

class QXRD_EXPORT QxrdFileBrowserWidget : public QWidget, public Ui::QxrdFileBrowserWidget
{
  Q_OBJECT

public:
  explicit QxrdFileBrowserWidget(QWidget *parent = 0);
  ~QxrdFileBrowserWidget();
  void initialize(QxrdFileBrowserSettingsWPtr settings,
            QxrdExperimentWPtr          experiment,
            QxrdDataProcessorWPtr       processor);

protected:
  void changeEvent(QEvent *e);

public slots:
  void onFilterChanged(int newfilter);
  void onSelectorChanged(QString str);
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
  void doSumImages();
  void doClearAccumulator();
  void doIntegrateAndAccumulate();
  void doSaveAccumulator();
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
  QxrdExperimentWPtr           m_Experiment;
  QxrdDataProcessorWPtr        m_Processor;
  QxrdFileBrowserModelPtr      m_Model;
  QStringList                  m_DirectoryStack;
};

#endif // QXRDFILEBROWSERWIDGET_H
