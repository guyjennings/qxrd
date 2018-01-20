#include "qxrdhistogramcommand.h"
#include <QToolButton>
#include "qxrdhistogramselector.h"
#include "qxrdimageplot.h"

QxrdHistogramCommand::QxrdHistogramCommand(QString                    name,
                                           QxrdPlotWidget            *plot,
                                           QxrdPlotWidgetSettingsWPtr set)
  : QxrdPlotButtonCommand(name, plot, set,
                          ":/images/histogram.png",
                          "Histogram",
                          true),
    m_HistogramSelector(NULL)
{
  connect(m_ToolButton,  &QToolButton::clicked,
          this,          &QxrdHistogramCommand::enable);

  m_HistogramSelector = new QxrdHistogramSelector(m_Plot);
  m_HistogramSelector -> setEnabled(false);

  setPlotPicker(m_HistogramSelector);

  connect(m_HistogramSelector, (void (QcepPlotMeasurer::*)( const QRectF &)) &QwtPlotPicker::selected,
          m_Plot, &QxrdImagePlot::selectHistogram);
}

void QxrdHistogramCommand::enable()
{
  QxrdPlotButtonCommand::enable();

  m_HistogramSelector -> setEnabled(true);
}

void QxrdHistogramCommand::disable()
{
  QxrdPlotButtonCommand::disable();

  m_HistogramSelector -> setEnabled(false);
}
