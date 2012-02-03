#include "qxrdpowderfitresultspage.h"

QxrdPowderFitResultsPage::QxrdPowderFitResultsPage(QxrdSettingsSaverPtr saver, QxrdDataProcessorPtr proc, QWidget *parent) :
    QxrdPowderFitRefinePage(saver, proc, parent)
{
}

void QxrdPowderFitResultsPage::selectResultsPage()
{
  m_PowderFitSteps->setCurrentIndex(2);

  removeGraphMarkers();
}
