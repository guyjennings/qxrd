/******************************************************************
*
*  $Id: qxrdpowderfitrefinepage.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPOWDERFITREFINEPAGE_H
#define QXRDPOWDERFITREFINEPAGE_H

#include "qxrdpowderfitidentifypage.h"

class QxrdPowderFitRefinePage : public QxrdPowderFitIdentifyPage
{
  Q_OBJECT;

public:
  QxrdPowderFitRefinePage(QxrdDataProcessorPtr proc, QWidget *parent);

public slots:
  void selectRefinePage();
  void appendRingFit();
  void removeRingFit();

protected:
  void appendFitParameter(int n, QxrdFitParameter *parm);
  void appendStaticFitParameter(QxrdFitParameter *parm);
  void appendRingFitParameter(int n, QxrdFitParameter *parm);
  void removeRingFitParameter(int n);

private:
  QxrdRingSetFitParametersPtr m_RingSet;
  int                         m_NStatic;
  int                         m_NRings;

private:
  HEADER_IDENT("$Id: qxrdpowderfitrefinepage.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPOWDERFITREFINEPAGE_H

/******************************************************************
*
*  $Log: qxrdpowderfitrefinepage.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/11 21:44:03  jennings
*  Work on powder ring fitting
*
*  Revision 1.1.2.1  2010/08/10 20:41:56  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*
*******************************************************************/
