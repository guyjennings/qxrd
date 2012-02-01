#include "qxrdpowderfitdialog.h"

QxrdPowderFitDialog::QxrdPowderFitDialog(QxrdSettingsSaver *saver, QxrdDataProcessorPtr proc, QWidget *parent) :
    QxrdPowderFitResultsPage(saver, proc, parent)
{
  connect(m_PowderFitSteps, SIGNAL(currentChanged(int)), this, SLOT(displayPageChanged(int)));
  connect(m_IdNextStepButton, SIGNAL(clicked()), this, SLOT(selectRefinePage()));
  connect(m_RefPrevStepButton, SIGNAL(clicked()), this, SLOT(selectIdentifyPage()));
  connect(m_RefNextStepButton, SIGNAL(clicked()), this, SLOT(selectResultsPage()));
  connect(m_ResPrevStepButton, SIGNAL(clicked()), this, SLOT(selectRefinePage()));

  selectIdentifyPage();
}

void QxrdPowderFitDialog::displayPageChanged(int page)
{
//  appendOutputString(tr("Display Page Changed to %1").arg(page));
//
  switch (page) {
  case 0:
    selectIdentifyPage();
    break;

  case 1:
    selectRefinePage();
    break;

  case 2:
    selectResultsPage();
    break;
  }
}
