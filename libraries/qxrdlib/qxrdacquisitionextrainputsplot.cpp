#include "qxrdacquisitionextrainputsplot.h"
#include "qwt_symbol.h"

QxrdAcquisitionExtraInputsPlot::QxrdAcquisitionExtraInputsPlot(QWidget *parent) :
  QcepPlot(parent)
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

void QxrdAcquisitionExtraInputsPlot::initialize(QcepPlotSettingsWPtr settings)
{
  QcepPlot::initialize(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::BottomLegend);
  }
}

void QxrdAcquisitionExtraInputsPlot::setNChannels(int nch)
{
  if (m_RawData.count() > nch) {
    for (int i=nch; i<m_RawData.count(); i++) {
      m_RawData[i]->detach();
      delete m_RawData[i];
    }

    m_RawData.resize(nch);
  } else if (m_RawData.count() < nch) {
    for (int i=m_RawData.count(); i<nch; i++) {
      QwtPlotCurve *pc = new QwtPlotCurve(tr("Chan %1").arg(i));
      m_RawData.append(pc);

      pc->setPen(QPen(m_Colors[i % m_Colors.count()]));

      pc->attach(this);
    }
  }

  if (m_Selections.count() > nch) {
    for (int i=nch; i<m_Selections.count(); i++) {
      m_Selections[i]->detach();
      delete m_Selections[i];
    }

    m_Selections.resize(nch);
  } else if (m_Selections.count() < nch) {
    for (int i=m_Selections.count(); i<nch; i++) {
      QwtPlotCurve *pc = new QwtPlotCurve(tr("Chan %1").arg(i));
      m_Selections.append(pc);

      QPen pen(m_Colors[i % m_Colors.count()]);
      QBrush brush(m_Colors[i % m_Colors.count()]);

      QwtSymbol *a = new QwtSymbol(QwtSymbol::Rect, brush, pen, QSize(3,3));

      pc->setStyle(QwtPlotCurve::NoCurve);
      pc->setSymbol(a);

      pc->attach(this);
    }
  }
}

void QxrdAcquisitionExtraInputsPlot::plotChannel(int ch, int i0, int i1, QcepDoubleVector x, QcepDoubleVector y)
{
  QwtPlotCurve *pc = m_RawData.value(ch);

  if (pc) {
    pc -> setSamples(x, y);
  }

  QwtPlotCurve *pc2 = m_Selections.value(ch);

  if (pc2) {
    pc2->setSamples(x.mid(i0, i1-i0), y.mid(i0, i1-i0));
  }
}
