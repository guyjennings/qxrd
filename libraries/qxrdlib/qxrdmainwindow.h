#ifndef QXRDMAINWINDOW_H
#define QXRDMAINWINDOW_H

#include "qxrdlib_global.h"
#include "qcepmainwindow.h"
#include "qxrdappcommon-ptr.h"
#include "qcepexperiment-ptr.h"
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

public slots:
  void saveExperiment();
  void saveExperimentCopy();
  void saveExperimentAs();
  void doEditPreferences();
  void doEditExposurePreferences();
  void doEditDetectorPreferences();

protected:
  void populateWindowsMenu();

private:
  void updateTitle();
  void acquireStarted();
  void acquiredFrame(QString fileName, int isum, int nsum, int iframe, int nframe, int igroup, int ngroup);
  void acquireComplete();
};

#endif // QXRDMAINWINDOW_H
