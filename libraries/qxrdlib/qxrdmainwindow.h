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

private slots:
  void populateWindowsMenu();
  void populateRecentExperimentsMenu();

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
};

#endif // QXRDMAINWINDOW_H
