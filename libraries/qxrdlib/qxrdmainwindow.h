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
  void populateWindowsMenu();
  void newWindow();

protected:
  QString             m_Name;
  QxrdApplicationWPtr m_Application;
  QxrdExperimentWPtr  m_Experiment;
  QMenu              *m_FileMenuP;
  QMenu              *m_EditMenuP;
  QMenu              *m_WindowMenuP;
};

#endif // QXRDMAINWINDOW_H
