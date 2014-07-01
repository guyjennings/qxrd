#include "qxrddistortioncorrectionplot.h"
#include "qxrdwindow.h"

QxrdDistortionCorrectionPlot::QxrdDistortionCorrectionPlot(QWidget *parent) :
  QxrdImagePlot(parent),
  m_ObjectNamer(this, "distortionCorrectionPlot"),
  m_Window(),
//  m_DataProcessor(),
  m_DistortionCorrection(),
  m_FirstTime(true)
{
}

void QxrdDistortionCorrectionPlot::init(QxrdImagePlotSettingsWPtr settings)
{
  QxrdImagePlot::init(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::RightLegend);
  }
}

void QxrdDistortionCorrectionPlot::setWindow(QxrdWindow *win)
{
  m_Window = win;

  QxrdWindow *wp = m_Window;

  if (wp) {
    setProcessor(wp -> dataProcessor());
  }

  QxrdDataProcessorPtr dp(processor());

  if (dp) {
    m_DistortionCorrection = dp -> distortionCorrection();

//    connect(m_Measurer, SIGNAL(selected(QVector<QPointF>)),
//            dp.data(), SLOT(printMeasuredPolygon(QVector<QPointF>)));

//    QxrdCenterFinderPtr cf(m_CenterFinder);

//    if (cf) {
//      connect(cf.data(), SIGNAL(parameterChanged()), this, SLOT(onParameterChanged()));
//    }
  }
}
