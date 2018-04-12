#include "qxrdextraoutputsplotwidget.h"
#include "qxrdextraoutputsplotwidgetsettings.h"
#include "qxrdsynchronizedoutputchannel.h"
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

QxrdExtraOutputsPlotWidget::QxrdExtraOutputsPlotWidget(QWidget *parent)
  : QcepPlotWidget(parent)
{
  m_Colors << Qt::black
           << QColor(170,110,40)
           << Qt::red
           << QColor(255,180,60)
           << Qt::yellow
           << Qt::green
           << Qt::blue
           << Qt::magenta
           << Qt::gray;
}

void QxrdExtraOutputsPlotWidget::initialize(QxrdExtraOutputsPlotWidgetSettingsWPtr settings)
{
  QcepPlotWidget::initialize(settings);

  addPlotCommandSpacer();
}

void QxrdExtraOutputsPlotWidget::clear()
{
  for (int i=0; i<m_Curves.count(); i++) {
    QwtPlotCurve *pc = m_Curves.value(i);

    if (pc) {
      pc->detach();
      delete pc;
    }
  }

  m_Curves.resize(0);
}

void QxrdExtraOutputsPlotWidget::plotChannel(QxrdSynchronizedOutputChannelWPtr chan)
{
  QxrdSynchronizedOutputChannelPtr out(chan);

  if (out) {
    int n = m_Curves.count();

    QwtPlotCurve *pc = new QwtPlotCurve(out->get_Name());

    if (pc) {
      m_Curves.append(pc);
      QPen pen(m_Colors[n % m_Colors.count()]);
      QBrush brush(m_Colors[n % m_Colors.count()]);

      QwtSymbol *a = new QwtSymbol(QwtSymbol::Rect, brush, pen, QSize(3,3));

//      pc->setStyle(QwtPlotCurve::NoCurve);
      pc->setPen(pen);
      pc->setSymbol(a);

      pc->setSamples(out->get_TimeValues(), out->get_Waveform());

      pc->attach(m_Plot);
    }
  }
}

void QxrdExtraOutputsPlotWidget::replot()
{
  m_Plot->replot();
}
