#include "qxrdpowderfitresultspage.h"

QxrdPowderFitResultsPage::QxrdPowderFitResultsPage(QxrdDataProcessorPtr proc, QWidget *parent) :
    QxrdPowderFitRefinePage(proc, parent)
{
}

void QxrdPowderFitResultsPage::selectResultsPage()
{
  m_PowderFitSteps->setCurrentIndex(2);

  removeGraphMarkers();
}
