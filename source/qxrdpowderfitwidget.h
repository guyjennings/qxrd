/******************************************************************
*
*  $Id: qxrdpowderfitwidget.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPOWDERFITWIDGET_H
#define QXRDPOWDERFITWIDGET_H

#include "qcepmacros.h"
#include <QDialog>
#include <QSignalMapper>
#include <QVector>
#include "qxrdforwardtypes.h"
#include "qxrdringsetfitparameters.h"
#include "ui_qxrdpowderfitwidget.h"
#include "qwt_double_rect.h"
#include "qwt_symbol.h"

class QxrdPowderFitWidget : public QDialog, public Ui::QxrdPowderFitWidget {
  Q_OBJECT;
public:
  QxrdPowderFitWidget(QxrdDataProcessorPtr proc, QWidget *parent = 0);
  ~QxrdPowderFitWidget();

protected:
  void changeEvent(QEvent *e);

public slots:

  void performFit();
  void cancelFit();
  void undoFit();

  void appendOutputString(QString msg);
  void removeGraphMarkers();
  void appendGraphMarker(int n, QwtDoublePoint pt);

protected:

protected:
  QxrdDataProcessorPtr     m_Processor;
  QVector<QwtSymbol::Style> m_SymbolStyles;

  HEADER_IDENT("$Id: qxrdpowderfitwidget.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPOWDERFITWIDGET_H

/******************************************************************
*
*  $Log: qxrdpowderfitwidget.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.5  2010/09/11 21:44:04  jennings
*  Work on powder ring fitting
*
*  Revision 1.1.2.4  2010/08/10 20:41:56  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*  Revision 1.1.2.3  2010/08/09 21:53:58  jennings
*  Made center and fit refiner into a modal dialog, partial implementation
*
*  Revision 1.1.2.2  2010/08/09 02:45:38  jennings
*  *** empty log message ***
*
*  Revision 1.1.2.1  2010/08/06 21:09:35  jennings
*  Initial partial implementation of powder ring fitting widget
*
*
*******************************************************************/
