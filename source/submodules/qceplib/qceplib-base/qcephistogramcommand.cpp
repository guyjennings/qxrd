#include "qcephistogramcommand.h"
#include <QToolButton>
#include "qcephistogramselector.h"
#include "qcepimageplot.h"

QcepHistogramCommand::QcepHistogramCommand(QcepPlotWidget            *plot,
                                           QcepPlotWidgetSettingsWPtr set)
  : QcepPlotButtonCommand("histogramCommand",
                          "Histogram",
                          plot,
                          set,
                          ":/images/histogram.png",
                          true),
    m_HistogramSelector(NULL)
{
  connect(m_ToolButton,  &QToolButton::clicked,
          this,          &QcepHistogramCommand::enable);

  m_HistogramSelector = new QcepHistogramSelector(m_Plot);
  m_HistogramSelector -> setEnabled(false);

  setPlotPicker(m_HistogramSelector);

  connect(m_HistogramSelector, (void (QcepPlotMeasurer::*)( const QRectF &)) &QwtPlotPicker::selected,
          m_Plot, &QcepImagePlot::selectHistogram);
}

void QcepHistogramCommand::enable()
{
  QcepPlotButtonCommand::enable();

  m_HistogramSelector -> setEnabled(true);
}

void QcepHistogramCommand::disable()
{
  QcepPlotButtonCommand::disable();

  m_HistogramSelector -> setEnabled(false);
}
