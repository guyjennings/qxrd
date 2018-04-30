#ifndef QXRDEXPERIMENTTHREAD_H
#define QXRDEXPERIMENTTHREAD_H

#include "qxrdlib_global.h"
#include "qcepthread.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdexperimentthread-ptr.h"
#include "qxrdappcommon-ptr.h"
#include "qxrdexperimentsettings-ptr.h"

class QXRD_EXPORT QxrdExperimentThread : public QcepThread
{
  Q_OBJECT

private:
  typedef QcepThread inherited;

public:
  //TODO: pass parent pointer on initialization...
  Q_INVOKABLE QxrdExperimentThread(QString name);

  ~QxrdExperimentThread();

  void haltExperiment();

  void initialize(QcepObjectWPtr parent);
  void initialize(QcepObjectWPtr            parent,
                  QString                   path,
                  QxrdExperimentSettingsPtr set,
                  int                       mode);

  static QxrdExperimentThreadWPtr findExperimentThread(QcepObjectWPtr parent);

  QxrdExperimentPtr experiment() const;

protected:
  void run();

private:
  QxrdExperimentPtr         m_Experiment;
  QString                   m_Path;
  QxrdExperimentSettingsPtr m_Settings;
  int                       m_ExperimentMode;
};

#endif // QXRDEXPERIMENTTHREAD_H
