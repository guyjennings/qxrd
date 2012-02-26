#include "qxrdpowderfitwidget.h"
#include "qwt_symbol.h"
#include "qwt_plot_item.h"
#include "qwt_plot_marker.h"
#include "qxrddataprocessor.h"

QxrdPowderFitWidget::QxrdPowderFitWidget(QxrdDataProcessorWPtr proc, QWidget *parent) :
    QDialog(parent),
    m_Processor(proc)
{
  setupUi(this);

  m_SymbolStyles << QwtSymbol::Ellipse
      << QwtSymbol::Cross << QwtSymbol::Diamond << QwtSymbol::Triangle;

  connect(m_PerformFitButton, SIGNAL(clicked()), this, SLOT(performFit()));
  connect(m_CancelFitButton, SIGNAL(clicked()), this, SLOT(cancelFit()));
  connect(m_UndoFitButton, SIGNAL(clicked()), this, SLOT(undoFit()));

  QxrdDataProcessorPtr dp(m_Processor);

  if (dp) {
    m_ImagePlot->onProcessedImageAvailable(dp->data(), QxrdMaskDataPtr(NULL));
  }
}

QxrdPowderFitWidget::~QxrdPowderFitWidget()
{
}

void QxrdPowderFitWidget::changeEvent(QEvent *e)
{
  QDialog::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    retranslateUi(this);
    break;
  default:
    break;
  }
}

void QxrdPowderFitWidget::performFit()
{
}

void QxrdPowderFitWidget::cancelFit()
{
}

void QxrdPowderFitWidget::undoFit()
{
}

void QxrdPowderFitWidget::appendOutputString(QString msg)
{
  m_FittingOutput->append(msg);
}

void QxrdPowderFitWidget::removeGraphMarkers()
{
  m_ImagePlot -> detachItems(QwtPlotItem::Rtti_PlotMarker);
  m_ImagePlot -> replot();
}

void QxrdPowderFitWidget::appendGraphMarker(int n, QwtDoublePoint pt)
{
  QwtPlotMarker *marker = new QwtPlotMarker();
  marker -> setValue(pt);
  QwtSymbol symb;

  symb.setStyle(m_SymbolStyles[n % m_SymbolStyles.count()]);

  symb.setPen(QPen(Qt::red));
  symb.setBrush(QBrush(Qt::red));
  symb.setSize(5,5);

  marker -> setSymbol(symb);

  marker -> attach(m_ImagePlot);
}
