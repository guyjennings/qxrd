#include "qswexperiment.h"
#include "qcepimagedata-ptr.h"

#ifdef Q_OS_WIN
#include "StellarNet.h"
#endif

QswExperiment::QswExperiment(QString name)
  : inherited(name),
    m_IntegrationPeriod(100),
    m_ScanAverages(1),
    m_IntegrationTime(this, "integrationTime", 0.5, "Integration Time"),
    m_AveragedTraces (this, "averagedTraces",  1,   "Averaged Traces")
{
}

void QswExperiment::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

#ifdef Q_OS_WIN
  OpenStellarNetLibrary();
#endif

  connect(&m_Timer, &QTimer::timeout,
          this,     &QswExperiment::onTimerTimeout);

  updateReadoutRate();
}

void QswExperiment::onIntegrationPeriodChanged(double per)
{
  m_IntegrationPeriod = (int) per*1000;

  updateReadoutRate();
}

void QswExperiment::onScanAveragingChanged(int avg)
{
  m_ScanAverages = avg;

  updateReadoutRate();
}

void QswExperiment::onTimerTimeout()
{
  QVector<float> s(2051);

#ifdef Q_OS_WIN
  ReadSpectrometer(NULL, 1, s.data());
#endif

  s.resize(2048);

  emit dataAvailable(s);
}

void QswExperiment::updateReadoutRate()
{
#ifdef Q_OS_WIN
  InitializeSpectrometer(m_IntegrationPeriod, m_ScanAverages, 1, 0, 0);
#endif

  m_Timer.start(m_IntegrationPeriod*m_ScanAverages);
}

void QswExperiment::saveExperiment()
{
}

void QswExperiment::saveExperimentAs(QString path)
{
}

void QswExperiment::saveExperimentCopyAs(QString path)
{
}

QString QswExperiment::experimentFilePath() const
{
  return "";
}

void QswExperiment::setExperimentFilePath(QString path)
{
}

void QswExperiment::plotImage(QcepDoubleImageDataPtr img)
{
}
