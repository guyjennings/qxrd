#include "qxrdmainwindowsettings.h"
#include "qxrdmainwindow.h"
#include "qxrdappcommon.h"
#include "qxrdexperiment.h"
#include "qxrdacqcommon.h"
#include "qxrdprocessor.h"

QxrdMainWindowSettings::QxrdMainWindowSettings(QString name, QString desc)
  : inherited(name, desc)
{
}

void QxrdMainWindowSettings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Application = QxrdAppCommon::findApplication(parent);
  m_Experiment  = QxrdExperiment::findExperiment(parent);
  m_Acquisition = QxrdAcqCommon::findAcquisition(parent);
  m_Processor   = QxrdProcessor::findProcessor(parent);

  if (m_Application == NULL) {
    printMessage("QxrdMainWindowSettings::m_Application == NULL");
  }

  if (m_Experiment  == NULL) {
    printMessage("QxrdMainWindowSettings::m_Experiment == NULL");
  }

  if (m_Acquisition == NULL) {
    printMessage("QxrdMainWindowSettings::m_Acquisition == NULL");
  }

  if (m_Processor   == NULL) {
    printMessage("QxrdMainWindowSettings::m_Processor == NULL");
  }
}
