#include "qxrddistortioncorrectionplot.h"
#include "qxrdwindow.h"

QxrdDistortionCorrectionPlot::QxrdDistortionCorrectionPlot(QWidget *parent) :
  QcepPlot(parent),
  m_ObjectNamer(this, "distortionCorrectionPlot"),
  m_Window(),
  m_FirstTime(true)
{
}

void QxrdDistortionCorrectionPlot::init(QcepPlotSettingsWPtr settings)
{
  QcepPlot::init(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::RightLegend);
  }
}

void QxrdDistortionCorrectionPlot::setWindow(QxrdWindow *win)
{
  m_Window = win;
}
