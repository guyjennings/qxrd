#ifndef QCEPMAINWINDOW_H
#define QCEPMAINWINDOW_H

#include "qceplib_global.h"
#include <QMainWindow>
#include <QDateTime>
#include "qcepobject-ptr.h"
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include "qcepmainwindowsettings-ptr.h"

class QCEP_EXPORT QcepMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit QcepMainWindow(QWidget *parent = 0);
  virtual ~QcepMainWindow();

  virtual void initialize(QcepObjectWPtr parent);

  void possiblyClose();
  bool wantToClose();

  virtual void setupMenus(QMenu *file, QMenu *edit, QMenu *window);

  virtual void saveExperimentCopy() = 0;
  virtual void saveExperimentAs() = 0;
  virtual void doEditPreferences() = 0;

signals:

public slots:
  void newWindow(QcepMainWindowSettingsWPtr set);

  void shrinkPanels(int fontSize, int spacing);
  void setFontSize(int fontSize);
  void setSpacing(int spacing);
  virtual void printLine(QString line) = 0;
  virtual void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void criticalMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;
  virtual void statusMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) = 0;

  void displayStatusMessage(QString msg);

protected:
  void shrinkDockWidget(QDockWidget *dockWidget, int fontSize, int spacing);
  void shrinkObject(QObject *obj, int fontSize, int spacing);
  void setObjectSpacing(QObject *obj, int spacing);

#ifndef QT_NO_DEBUG
  void checkObjectInitialization() const;
#endif

  virtual void populateWindowsMenu();
  void appendToWindowMenu(QMenu* wmenu, QcepMainWindowSettingsWPtr s);

private:
  void doTimerUpdate();
  void clearStatusMessage();
  virtual void updateTitle();
  void onUpdateIntervalMsecChanged(int newVal);
  void allocatedMemoryChanged();

  void populateEditMenu();
  void populateRecentExperimentsMenu();

  void doUndo();
  void doRedo();
  void doCut();
  void doCopy();
  void doPaste();
  void doDelete();
  void doSelectAll();

protected:
  bool                m_Initialized;
  QcepObjectWPtr      m_Parent;

  QLabel             *m_StatusMsg;
  QProgressBar       *m_Progress;
  QProgressBar       *m_AllocationStatus;
  QTimer              m_StatusTimer;
  QTimer              m_UpdateTimer;

  QMenu              *m_FileMenuP;
  QMenu              *m_EditMenuP;
  QMenu              *m_WindowMenuP;
  QMenu              *m_RecentExperimentsMenu;

  QAction            *m_ActionNewExperiment;
  QAction            *m_ActionRecentExperiments;
  QAction            *m_ActionOpenExperiment;
  QAction            *m_ActionCloseExperiment;

  QAction            *m_ActionSaveExperiment;
  QAction            *m_ActionSaveExperimentAs;
  QAction            *m_ActionSaveExperimentCopy;

  QAction            *m_ActionGlobalPreferences;
  QAction            *m_ActionExperimentPreferences;
  QAction            *m_ActionQuit;

  QAction            *m_ActionUndo;
  QAction            *m_ActionRedo;
  QAction            *m_ActionCut;
  QAction            *m_ActionCopy;
  QAction            *m_ActionPaste;
  QAction            *m_ActionDelete;
  QAction            *m_ActionSelectAll;
};

#endif // QCEPMAINWINDOW_H
