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
  explicit QxrdMainWindow(QString name);
  void initialize(QcepObjectWPtr parent);

  void setupMenus(QMenu *file, QMenu *edit, QMenu *window);

  void printLine(QString line);
  void printMessage(QString msg, QDateTime ts = QDateTime::currentDateTime());
  void criticalMessage(QString msg, QDateTime ts = QDateTime::currentDateTime());
  void statusMessage(QString msg, QDateTime ts = QDateTime::currentDateTime());

public slots:
  void saveExperimentCopy();
  void saveExperimentAs();
  void doEditPreferences();
  void doEditDetectorPreferences();

protected:
  void populateWindowsMenu();

private:
  void updateTitle();
  void acquireStarted();
  void acquiredFrame(QString fileName, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireComplete();

protected:
  QString             m_Name;
};

#endif // QXRDMAINWINDOW_H
