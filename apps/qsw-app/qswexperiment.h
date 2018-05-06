#ifndef QSWEXPERIMENT_H
#define QSWEXPERIMENT_H

#include "qswlib_global.h"
#include "qcepexperiment.h"
#include <QVector>
#include <QTimer>

class /*QSW_EXPORT*/ QswExperiment : public QcepExperiment
{
  Q_OBJECT

private:
  typedef QcepExperiment inherited;

public:
  QswExperiment(QString name);

  void initialize(QcepObjectWPtr parent);

  void saveExperiment();
  void saveExperimentAs(QString path);
  void saveExperimentCopyAs(QString path);

public slots:
  void onIntegrationPeriodChanged(double per);
  void onScanAveragingChanged(int avg);
  void updateReadoutRate();

  QString experimentFilePath() const;
  void setExperimentFilePath(QString path);
  void plotImage(QcepDoubleImageDataPtr img);

signals:
  void dataAvailable(QVector<float> d);

private:
  void onTimerTimeout();

  QTimer m_Timer;

  int    m_IntegrationPeriod;
  int    m_ScanAverages;

public:
  Q_PROPERTY(double integrationTime READ get_IntegrationTime WRITE set_IntegrationTime)
  QCEP_DOUBLE_PROPERTY(IntegrationTime)

  Q_PROPERTY(int    averagedTraces  READ get_AveragedTraces WRITE set_AveragedTraces)
  QCEP_INTEGER_PROPERTY(AveragedTraces)
};

#endif // QSWEXPERIMENT_H
