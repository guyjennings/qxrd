#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepplot.h"
#include "qcepintegrateddata-ptr.h"

#include "qxrdprocessor-ptr.h"
#include "qxrdintegrator-ptr.h"

//TODO: is class used?
class QXRD_EXPORT QxrdIntegratorPlot : public QcepPlot
{
  Q_OBJECT

public:
  QxrdIntegratorPlot(QWidget *parent=0);
  //TODO: change to QObjectWPtr...
  void initialize(QcepPlotSettingsWPtr settings);

  void setProcessor(QxrdProcessorWPtr proc);

public slots:
  void onNewIntegrationAvailable(QcepIntegratedDataPtr data);
  void clearGraph();
  void clearSelectedCurves();

private:
  QxrdProcessorWPtr     m_Processor;
  QxrdIntegratorWPtr    m_Integrator;
  int                   m_PlotIndex;
  QString               m_XUnitsLabel;
};

#endif // QXRDINTEGRATORPLOT_H
