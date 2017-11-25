#include "qxrdhistogramcommand.h"
#include <QToolButton>
#include "qxrdhistogramselector.h"
#include "qxrdimageplot.h"

QxrdHistogramCommand::QxrdHistogramCommand(QString name, QxrdPlotWidget *plot)
  : QxrdPlotButtonCommand(name, plot),
    m_HistogramSelector(NULL)
{
  m_HistogramSelector = new QxrdHistogramSelector(m_Plot->canvas(), m_Plot);
  m_HistogramSelector -> setEnabled(false);

  connect(m_HistogramSelector, (void (QcepPlotMeasurer::*)( const QRectF &)) &QwtPlotPicker::selected,
          m_Plot, &QxrdImagePlot::selectHistogram);
}

QToolButton* QxrdHistogramCommand::toolButton()
{
  QToolButton* res = new QToolButton();

  res->setObjectName(get_Name());
  QIcon icon;

  icon.addFile(QStringLiteral(":/images/histogram.png"), QSize(), QIcon::Normal, QIcon::Off);
  res->setIcon(icon);
  res->setIconSize(QSize(24,24));
  res->setCheckable(true);
  res->setAutoExclusive(true);
  res->setToolTip(tr("Histogram"));

  connect(res,  &QToolButton::clicked,
          this, &QxrdHistogramCommand::enable);

  return res;
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
