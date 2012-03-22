#include "qxrdacquisitionextrainputsplot.h"

QxrdAcquisitionExtraInputsPlot::QxrdAcquisitionExtraInputsPlot(QWidget *parent) :
  QxrdPlot(parent)
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

void QxrdAcquisitionExtraInputsPlot::setNChannels(int nch)
{
  if (m_Channels.count() > nch) {
    for (int i=nch; i<m_Channels.count(); i++) {
      m_Channels[i]->detach();
      delete m_Channels[i];
    }

    m_Channels.resize(nch);
  } else if (m_Channels.count() < nch) {
    for (int i=m_Channels.count(); i<nch; i++) {
      QwtPlotCurve *pc = new QwtPlotCurve(tr("Chan %1").arg(i));
      m_Channels.append(pc);

      pc->setPen(QPen(m_Colors[i % m_Colors.count()]));

      pc->attach(this);
    }
  }
}

void QxrdAcquisitionExtraInputsPlot::plotChannel(int ch, QcepDoubleVector x, QcepDoubleVector y)
{
  QwtPlotCurve *pc = m_Channels.value(ch);

  if (pc) {
    pc -> setData(x, y);
  }
}
