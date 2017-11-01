#ifndef QXRDMAINWINDOWSETTINGS_H
#define QXRDMAINWINDOWSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmainwindowsettings.h"
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdMainWindowSettings : public QcepMainWindowSettings
{
  Q_OBJECT

public:
  QxrdMainWindowSettings(QString name,
                         QxrdApplicationPtr app,
                         QxrdExperimentPtr expt);

protected:
  QxrdApplicationPtr m_Application;
  QxrdExperimentPtr  m_Experiment;
};

Q_DECLARE_METATYPE(QxrdMainWindowSettings*)

#endif // QXRDMAINWINDOWSETTINGS_H
