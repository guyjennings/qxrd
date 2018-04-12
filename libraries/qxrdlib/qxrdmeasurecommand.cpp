#include "qxrdmeasurecommand.h"
#include <QToolButton>
#include "qxrdimageplotmeasurer.h"
#include "qcepimageplot.h"

QxrdMeasureCommand::QxrdMeasureCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("measureCommand", "Measure", plot, set, ":/images/measure.png", true)
{
  m_Measurer = new QxrdImagePlotMeasurer(m_Plot);
  m_Measurer -> setEnabled(false);

  setPlotPicker(m_Measurer);

  connect(m_Measurer, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QxrdMeasureCommand::selected);
}

//TODO: implement
void QxrdMeasureCommand::selected(const QVector<QPointF> &p)
{
  printf("QxrdMeasureCommand::selected\n");
//  connect(m_Measurer, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QwtPlotPicker::selected,
//          dp.data(), &QxrdProcessor::printMeasuredPolygon);
}
