#include "qxrdmeasurecommand.h"
#include <QToolButton>
#include "qxrdimageplotmeasurer.h"
#include "qxrdimageplot.h"

QxrdMeasureCommand::QxrdMeasureCommand(QString name, QxrdPlotWidget *plot, QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set, ":/images/measure.png", "Measure", true)
{
  m_Measurer = new QxrdImagePlotMeasurer(m_Plot->canvas(), m_Plot);
  m_Measurer -> setEnabled(false);

  setPlotPicker(m_Measurer);

  connect(m_Measurer, (void (QwtPlotPicker::*)(const QVector<QPointF> &)) & QwtPlotPicker::selected,
          this, &QxrdMeasureCommand::selected);
}

//TODO: implement
void QxrdMeasureCommand::selected(const QVector<QPointF> &p)
{
  printf("QxrdMeasureCommand::selected\n");
}
