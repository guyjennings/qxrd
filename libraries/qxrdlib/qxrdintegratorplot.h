#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include "qcepmacros.h"
#include "qcepplot.h"
#include "qcepintegrateddata-ptr.h"

#include "qxrddataprocessor.h"
#include "qxrdintegrator.h"

class QxrdIntegratorPlot : public QcepPlot
{
  Q_OBJECT

public:
  QxrdIntegratorPlot(QWidget *parent=0);
  void init(QcepPlotSettingsWPtr settings);

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
