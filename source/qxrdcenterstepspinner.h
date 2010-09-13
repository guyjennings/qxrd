/******************************************************************
*
*  $Id: qxrdcenterstepspinner.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERSTEPSPINNER_H
#define QXRDCENTERSTEPSPINNER_H

#include "qcepmacros.h"
#include <QDoubleSpinBox>

class QxrdCenterStepSpinner : public QDoubleSpinBox
{
  Q_OBJECT
public:
  explicit QxrdCenterStepSpinner(QWidget *parent = 0);

  virtual void stepBy(int steps);

signals:
public slots:
private:
  HEADER_IDENT("$Id: qxrdcenterstepspinner.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif // QXRDCENTERSTEPSPINNER_H

/******************************************************************
*
*  $Log: qxrdcenterstepspinner.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/09/10 18:53:32  jennings
*  1:2:5 step spinner
*
*
*******************************************************************/

