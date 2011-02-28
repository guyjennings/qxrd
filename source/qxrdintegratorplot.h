#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include "qcepmacros.h"

#include "qxrdintegrateddata.h"
#include "qxrdplot.h"

class QxrdDataProcessor;
class QxrdIntegrator;

class QxrdIntegratorPlot : public QxrdPlot
{
  Q_OBJECT;

public:
  QxrdIntegratorPlot(QWidget *parent=0);

  void setDataProcessor(QxrdDataProcessor *proc);

public slots:
  void onNewIntegrationAvailable(QxrdIntegratedDataPtr data);
  void clearGraph();

private:
  QxrdDataProcessor *m_DataProcessor;
  QxrdIntegrator    *m_Integrator;
  int                m_PlotIndex;
};

#endif // QXRDINTEGRATORPLOT_H
