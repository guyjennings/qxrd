#ifndef QCEPFILEBROWSERWIDGET_H
#define QCEPFILEBROWSERWIDGET_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include <QWidget>
#include "ui_qcepfilebrowserwidget.h"
#include "qcepfilebrowsersettings-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qcepfilebrowsermodel-ptr.h"
#include <QFileInfo>
#include <QMutex>

class QXRD_EXPORT QcepFileBrowserWidget : public QWidget, public Ui::QcepFileBrowserWidget
{
  Q_OBJECT

public:
  explicit QcepFileBrowserWidget(QWidget *parent = 0);
  ~QcepFileBrowserWidget();
  //TODO: change to QcepObjectWPtr
  void initialize(QcepFileBrowserSettingsWPtr settings,
            QxrdExperimentWPtr          experiment,
            QxrdProcessorWPtr processor);
  void haltBrowser();

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
  QcepFileBrowserSettingsWPtr  m_FileBrowserSettings;
  mutable QMutex               m_Mutex;
  QxrdExperimentWPtr           m_Experiment;
  QxrdProcessorWPtr            m_Processor;
  QcepFileBrowserModelPtr      m_Model;
  QStringList                  m_DirectoryStack;
};

#endif // QCEPFILEBROWSERWIDGET_H
