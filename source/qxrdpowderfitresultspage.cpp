#include "qxrdpowderfitresultspage.h"

QxrdPowderFitResultsPage::QxrdPowderFitResultsPage(QxrdSettingsSaverWPtr saver, QxrdDataProcessorWPtr proc, QWidget *parent) :
    QxrdPowderFitRefinePage(saver, proc, parent)
{
}

void QxrdPowderFitResultsPage::selectResultsPage()
{
  m_PowderFitSteps->setCurrentIndex(2);

  removeGraphMarkers();
}
