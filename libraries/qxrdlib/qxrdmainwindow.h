#ifndef QXRDMAINWINDOW_H
#define QXRDMAINWINDOW_H

#include "qxrdlib_global.h"
#include "qcepmainwindow.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdprocessor-ptr.h"
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QStatusBar>

class QXRD_EXPORT QxrdMainWindow : public QcepMainWindow
{
  Q_OBJECT

private:
  typedef QcepMainWindow inherited;

public:
  explicit QxrdMainWindow(QString name,
                          QxrdAppCommonWPtr app,
                          QxrdExperimentWPtr expt,
                          QxrdAcqCommonWPtr acqw,
                          QxrdProcessorWPtr procw);

  void setupMenus(QMenu *file, QMenu *edit, QMenu *window);

  void printLine(QString line);
  void printMessage(QString msg, QDateTime ts);
  void criticalMessage(QString msg, QDateTime ts);
  void statusMessage(QString msg, QDateTime ts);

public slots:
  void saveExperimentCopy();
  void saveExperimentAs();
  void doEditPreferences();
  void doEditDetectorPreferences();

private:
  void updateTitle();
  void acquireStarted();
  void acquiredFrame(QString fileName, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireComplete();

protected:
  QString             m_Name;
  QxrdAppCommonWPtr   m_Application;
  QxrdExperimentWPtr  m_Experiment;
  QxrdAcqCommonWPtr   m_Acquisition;
  QxrdProcessorWPtr   m_Processor;
};

#endif // QXRDMAINWINDOW_H
