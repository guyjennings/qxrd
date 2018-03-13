#ifndef QXRDEXPERIMENTSETTINGS_H
#define QXRDEXPERIMENTSETTINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include <QSettings>
#include "qxrdexperiment-ptr.h"

class QXRD_EXPORT QxrdExperimentSettings : public QSettings
{
  Q_OBJECT

public:
  QxrdExperimentSettings(QxrdExperiment *parent=0);
  QxrdExperimentSettings(const QString &path, QSettings::Format format = QSettings::IniFormat, QcepObject *parent=0);
  ~QxrdExperimentSettings();

  static QString latestAutoPath(QString path);

private:
  QxrdExperiment *m_Experiment;
};

#endif
