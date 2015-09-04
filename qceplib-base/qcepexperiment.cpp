#include "qcepexperiment.h"
#include "qcepsettingssaver.h"
#include <QFileInfo>
#include <QDir>

QcepExperiment::QcepExperiment(QString path, QString name, QcepObject *parent) :
  QcepObject(name, parent),
  m_SettingsSaver(new QcepSettingsSaver(this)),
  m_ExperimentKind(m_SettingsSaver, this, "experimentKind", -1, "Kind of Experiment"),
  m_ExperimentDirectory(QcepSettingsSaverPtr(), this, "experimentDirectory", defaultExperimentDirectory(path), "Experiment Directory"),
  m_ExperimentFileName(QcepSettingsSaverPtr(), this, "experimentFileName", defaultExperimentFileName(path), "Experiment File"),
  m_ExperimentName(QcepSettingsSaverPtr(), this, "experimentName", defaultExperimentName(path), "Experiment Name"),
  m_ExperimentDescription(m_SettingsSaver, this, "experimentDescription", "", "Experiment Description")
{
}

QString QcepExperiment::defaultExperimentDirectory(QString path) const
{
  QFileInfo info(path);

  QString directory = info.dir().canonicalPath();

  return directory;
}

QString QcepExperiment::defaultExperimentFileName(QString path) const
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.fileName();
  } else {
    return info.fileName()+".qxrdp";
  }
}

QString QcepExperiment::defaultExperimentName(QString path) const
{
  QFileInfo info(path);

  if (info.suffix() == "qxrdp") {
    return info.completeBaseName();
  } else {
    return info.fileName();
  }
}

