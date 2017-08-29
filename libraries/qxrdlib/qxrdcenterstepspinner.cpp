#include "qxrdcenterstepspinner.h"
#include <qmath.h>
#include <stdio.h>

QxrdCenterStepSpinner::QxrdCenterStepSpinner(QWidget *parent) :
    QDoubleSpinBox(parent)
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
