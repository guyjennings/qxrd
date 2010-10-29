#include "qxrdexposuretimespinner.h"

QxrdExposureTimeSpinner::QxrdExposureTimeSpinner(QWidget *parent)
  : QDoubleSpinBox(parent)
{
  m_PreferredExposures << 0.06666667 << 0.1 << 0.2 << 0.5 << 1.0 << 2.0 << 5.0 << 8.0;
}

void QxrdExposureTimeSpinner::stepBy(int steps)
{
  double oldVal = value();

  int n = m_PreferredExposures.size();
  int foundAt = 0;

  for (int i=0; i<n; i++) {
    if (m_PreferredExposures[i] <= oldVal) {
      foundAt = i;
    }
  }

  int newFound = foundAt + steps;

  if (newFound < 0) {
    newFound = 0;
  } else if (newFound >= n) {
    newFound = n-1;
  }

  double newVal = m_PreferredExposures[newFound];

  setValue(newVal);

  emit valueChanged(newVal);
}
