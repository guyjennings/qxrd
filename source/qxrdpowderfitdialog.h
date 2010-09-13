/******************************************************************
*
*  $Id: qxrdpowderfitdialog.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPOWDERFITDIALOG_H
#define QXRDPOWDERFITDIALOG_H

#include "qxrdpowderfitresultspage.h"

class QxrdPowderFitDialog : public QxrdPowderFitResultsPage
{
  Q_OBJECT;
public:
  QxrdPowderFitDialog(QxrdDataProcessorPtr proc, QWidget *parent);

public slots:
  void displayPageChanged(int page);

protected:

private:
  HEADER_IDENT("$Id: qxrdpowderfitdialog.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPOWDERFITDIALOG_H

/******************************************************************
*
*  $Log: qxrdpowderfitdialog.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/10 20:41:55  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*
*******************************************************************/
