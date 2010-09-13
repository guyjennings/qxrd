/******************************************************************
*
*  $Id: qxrdpowderfitresultspage.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdpowderfitresultspage.h"

QxrdPowderFitResultsPage::QxrdPowderFitResultsPage(QxrdDataProcessorPtr proc, QWidget *parent) :
    QxrdPowderFitRefinePage(proc, parent),
    SOURCE_IDENT("$Id: qxrdpowderfitresultspage.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
{
}

void QxrdPowderFitResultsPage::selectResultsPage()
{
  m_PowderFitSteps->setCurrentIndex(2);

  removeGraphMarkers();
}

/******************************************************************
*
*  $Log: qxrdpowderfitresultspage.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/11 21:44:04  jennings
*  Work on powder ring fitting
*
*  Revision 1.1.2.1  2010/08/10 20:41:56  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*
*******************************************************************/
