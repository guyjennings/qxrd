#ifndef QXRDEXPERIMENTSETTINGS_H
#define QXRDEXPERIMENTSETTINGS_H

#include "qcepmacros.h"

#include <QSettings>

class QxrdExperiment;

class QxrdExperimentSettings : public QSettings
{
  Q_OBJECT

public:
  QxrdExperimentSettings(QxrdExperiment *parent=0);
  QxrdExperimentSettings(const QString &path, QSettings::Format format = QSettings::IniFormat, QObject *parent=0);
  ~QxrdExperimentSettings();

private:
  QxrdExperiment *m_Experiment;
};

#endif
