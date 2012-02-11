#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include "qcepmacros.h"

#include "qxrdintegrateddata.h"
#include "qxrddataprocessor.h"
#include "qxrdplot.h"

class QxrdIntegrator;
typedef QWeakPointer<QxrdIntegrator> QxrdIntegratorWPtr;

class QxrdIntegratorPlot : public QxrdPlot
{
  Q_OBJECT

public:
  QxrdIntegratorPlot(QWidget *parent=0);

  void setDataProcessor(QxrdDataProcessorWPtr proc);

public slots:
  void onNewIntegrationAvailable(QxrdIntegratedDataPtr data);
  void clearGraph();
  void clearSelectedCurves();

private:
  QxrdDataProcessorWPtr m_DataProcessor;
  QxrdIntegratorWPtr    m_Integrator;
  int                   m_PlotIndex;
  QString               m_XUnitsLabel;
};

#endif // QXRDINTEGRATORPLOT_H
