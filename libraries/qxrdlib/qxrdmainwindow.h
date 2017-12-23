#ifndef QXRDMAINWINDOW_H
#define QXRDMAINWINDOW_H

#include "qxrdlib_global.h"
#include "qcepmainwindow.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdMainWindow : public QcepMainWindow
{
  Q_OBJECT
public:
  explicit QxrdMainWindow(QString name,
                          QxrdApplicationWPtr app,
                          QxrdExperimentWPtr expt);

  virtual void setupMenus(QMenu *file, QMenu *edit, QMenu *window);

  void printLine(QString line);
  void printMessage(QString msg, QDateTime ts);
  void criticalMessage(QString msg, QDateTime ts);
  void statusMessage(QString msg, QDateTime ts);

public slots:
  void newWindow();
  void saveExperimentCopy();
  void saveExperimentAs();
  void doEditPreferences();
  void doEditDetectorPreferences();

private slots:
  void populateEditMenu();
  void populateWindowsMenu();
  void populateRecentExperimentsMenu();

  void doUndo();
  void doRedo();
  void doCut();
  void doCopy();
  void doPaste();
  void doDelete();
  void doSelectAll();

protected:
  QString             m_Name;
  QxrdApplicationWPtr m_Application;
  QxrdExperimentWPtr  m_Experiment;
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

#endif // QXRDMAINWINDOW_H
