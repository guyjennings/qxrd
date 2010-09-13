/******************************************************************
*
*  $Id: qxrdpowderfitdialog.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdpowderfitdialog.h"

QxrdPowderFitDialog::QxrdPowderFitDialog(QxrdDataProcessorPtr proc, QWidget *parent) :
    QxrdPowderFitResultsPage(proc, parent),
    SOURCE_IDENT("$Id: qxrdpowderfitdialog.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
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

/******************************************************************
*
*  $Log: qxrdpowderfitdialog.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/11 21:44:03  jennings
*  Work on powder ring fitting
*
*  Revision 1.1.2.1  2010/08/10 20:41:55  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*
*******************************************************************/
