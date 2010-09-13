/******************************************************************
*
*  $Id: qxrdcenterstepspinner.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterstepspinner.h"
#include <math.h>
#include <stdio.h>

QxrdCenterStepSpinner::QxrdCenterStepSpinner(QWidget *parent) :
    QDoubleSpinBox(parent),
    SOURCE_IDENT("$Id: qxrdcenterstepspinner.cpp,v 1.2 2010/09/13 20:00:39 jennings Exp $")
{
  setDecimals(5);
}

void QxrdCenterStepSpinner::stepBy(int steps)
{
  double newVal = value();
  double sgn    = (newVal < 0 ? -1 : 1);
  double logVal = log10(fabs(newVal));
  double frac   = logVal - floor(logVal);

//  printf("frac = %g\n", frac);
  double mul;
  if (frac < 0.2) {
    mul = (steps > 0 ? 2.0 : 2.0);
  } else if (frac < 0.5) {
    mul = (steps > 0 ? 2.5 : 2.0);
  } else {
    mul = (steps > 0 ? 2.0 : 2.5);
  }


  if (steps > 0) {
    if (newVal == 0) {
      newVal = 0.00001;
    } else {
      newVal *= mul;
    }
  } else if (steps < 0) {
    newVal /= mul;
  }

  setValue(newVal*sgn);
//
//  emit valueChanged(newVal);
}

/******************************************************************
*
*  $Log: qxrdcenterstepspinner.cpp,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/09/10 18:53:32  jennings
*  1:2:5 step spinner
*
*
*******************************************************************/

