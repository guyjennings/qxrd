#include "qxrdexperimentsettings.h"
#include <stdio.h>
#include "qxrdexperiment.h"

QxrdExperimentSettings::QxrdExperimentSettings(QxrdExperiment *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd-2",NULL),
    m_Experiment(parent)
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Experiment->printMessage("QxrdExperimentSettings::QxrdExperimentSettings");
  }
}

QxrdExperimentSettings::QxrdExperimentSettings(const QString &path, QSettings::Format format, QObject *parent)
  : QSettings(path, format, parent)
{

}

QxrdExperimentSettings::~QxrdExperimentSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    m_Experiment->printMessage("QxrdExperimentSettings::~QxrdExperimentSettings");
  }
}
