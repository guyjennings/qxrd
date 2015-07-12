#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include "qcepmacros.h"

#include "qcepintegrateddata.h"
#include "qxrddataprocessor.h"
#include "qxrdplot.h"
#include "qxrdintegrator.h"

class QxrdIntegratorPlot : public QxrdPlot
{
  Q_OBJECT

public:
  QxrdIntegratorPlot(QWidget *parent=0);
  void init(QxrdPlotSettingsWPtr settings);

  void setDataProcessor(QxrdDataProcessorWPtr proc);

public slots:
  void onNewIntegrationAvailable(QcepIntegratedDataPtr data);
  void clearGraph();
  void clearSelectedCurves();

private:
  QxrdDataProcessorWPtr m_DataProcessor;
  QxrdIntegratorWPtr    m_Integrator;
  int                   m_PlotIndex;
  QString               m_XUnitsLabel;
};

#endif // QXRDINTEGRATORPLOT_H
