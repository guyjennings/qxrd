#include "qxrddebug.h"
#include "qxrdexperimentsettings.h"
#include <stdio.h>
#include "qxrdexperiment.h"
#include <QFileInfo>

QxrdExperimentSettings::QxrdExperimentSettings(QxrdExperiment *parent)
  : QSettings("cep.xor.aps.anl.gov","qxrd-3",NULL),
    m_Experiment(parent)
{
  if (qcepDebug(DEBUG_PREFS)) {
    if (m_Experiment) {
      m_Experiment->printMessage("QxrdExperimentSettings::QxrdExperimentSettings");
    }
  }
}

QxrdExperimentSettings::QxrdExperimentSettings(const QString &path, QSettings::Format format, QcepObject *parent)
  : QSettings(path, format, parent),
    m_Experiment(NULL)
{
}

QxrdExperimentSettings::~QxrdExperimentSettings()
{
  if (qcepDebug(DEBUG_PREFS)) {
    if (m_Experiment) {
      m_Experiment->printMessage("QxrdExperimentSettings::~QxrdExperimentSettings");
    }
  }
}

QString QxrdExperimentSettings::latestAutoPath(QString path)
{
  QString res = path;

  QString autoPath1 = path+".auto";
  QString autoPath2 = path+".auto.bak";

  QFileInfo info(path);
  QFileInfo info1(autoPath1);
  QFileInfo info2(autoPath2);

  QDateTime dt  = info.lastModified();
  QDateTime dt1 = info1.lastModified();
  QDateTime dt2 = info2.lastModified();

  if (dt > dt1 && dt > dt2) {
    res = path;
  } else if (dt1 > dt2) {
    res = autoPath1;
  } else {
    res = autoPath2;
  }

  return res;
}
