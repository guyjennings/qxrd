#include "qcepmeasurecommand.h"
#include <QToolButton>
#include "qcepimageplotmeasurer.h"
#include "qcepimageplot.h"

QcepMeasureCommand::QcepMeasureCommand(QcepPlotWidget *plot, QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("measureCommand", "Measure", plot, set, ":/images/measure.png", true)
{
  m_Measurer = new QcepImagePlotMeasurer(m_Plot);
  m_Measurer -> setEnabled(false);

  setPlotPicker(m_Measurer);

  connect(m_Measurer, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QcepMeasureCommand::selected);
}

//TODO: implement
void QcepMeasureCommand::selected(const QVector<QPointF> &p)
{
  printf("QcepMeasureCommand::selected\n");
//  connect(m_Measurer, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QwtPlotPicker::selected,
//          dp.data(), &QcepProcessor::printMeasuredPolygon);
}
