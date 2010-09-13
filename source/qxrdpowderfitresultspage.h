/******************************************************************
*
*  $Id: qxrdpowderfitresultspage.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPOWDERFITRESULTSPAGE_H
#define QXRDPOWDERFITRESULTSPAGE_H

#include "qxrdpowderfitrefinepage.h"

class QxrdPowderFitResultsPage : public QxrdPowderFitRefinePage
{
  Q_OBJECT;
public:
  QxrdPowderFitResultsPage(QxrdDataProcessorPtr proc, QWidget *parent);

public slots:
  void selectResultsPage();

private:
  HEADER_IDENT("$Id: qxrdpowderfitresultspage.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPOWDERFITRESULTSPAGE_H

/******************************************************************
*
*  $Log: qxrdpowderfitresultspage.h,v $
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
