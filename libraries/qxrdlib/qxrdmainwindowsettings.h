#ifndef QXRDMAINWINDOWSETTINGS_H
#define QXRDMAINWINDOWSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmainwindowsettings.h"
#include "qxrdmainwindow-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdprocessor-ptr.h"

class QXRD_EXPORT QxrdMainWindowSettings : public QcepMainWindowSettings
{
  Q_OBJECT

private:
  typedef QcepMainWindowSettings inherited;

public:
  QxrdMainWindowSettings(QString name,
                         QString desc);

  void initialize(QcepObjectWPtr parent);

protected:
  QxrdAppCommonWPtr   m_Application;
  QxrdExperimentWPtr  m_Experiment;
  QxrdAcqCommonWPtr   m_Acquisition;
  QxrdProcessorWPtr   m_Processor;
};

Q_DECLARE_METATYPE(QxrdMainWindowSettings*)

#endif // QXRDMAINWINDOWSETTINGS_H
