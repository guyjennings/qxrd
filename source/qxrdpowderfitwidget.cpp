/******************************************************************
*
*  $Id: qxrdpowderfitwidget.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdpowderfitwidget.h"
#include "qxrddataprocessor.h"
#include "qwt_symbol.h"
#include "qwt_plot_item.h"
#include "qwt_plot_marker.h"

QxrdPowderFitWidget::QxrdPowderFitWidget(QxrdDataProcessorPtr proc, QWidget *parent) :
    QDialog(parent),
    m_Processor(proc),
    SOURCE_IDENT("$Id: qxrdpowderfitwidget.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
{
  setupUi(this);

  m_SymbolStyles << QwtSymbol::Ellipse
      << QwtSymbol::Cross << QwtSymbol::Diamond << QwtSymbol::Triangle;

  connect(m_PerformFitButton, SIGNAL(clicked()), this, SLOT(performFit()));
  connect(m_CancelFitButton, SIGNAL(clicked()), this, SLOT(cancelFit()));
  connect(m_UndoFitButton, SIGNAL(clicked()), this, SLOT(undoFit()));

  m_ImagePlot->onProcessedImageAvailable(m_Processor->data());
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

/******************************************************************
*
*  $Log: qxrdpowderfitwidget.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.7  2010/09/11 21:44:04  jennings
*  Work on powder ring fitting
*
*  Revision 1.1.2.6  2010/08/10 20:41:56  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*  Revision 1.1.2.5  2010/08/09 21:58:14  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.4  2010/08/09 21:53:58  jennings
*  Made center and fit refiner into a modal dialog, partial implementation
*
*  Revision 1.1.2.3  2010/08/09 17:01:36  jennings
*  Initial partial implementation of powder ring fitting widget
*
*  Revision 1.1.2.2  2010/08/09 02:45:38  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.1  2010/08/06 21:09:34  jennings
*  Initial partial implementation of powder ring fitting widget
*
*
*******************************************************************/
