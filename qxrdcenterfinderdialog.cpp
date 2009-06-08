#include "qxrdcenterfinderdialog.h"

QxrdCenterFinderDialog::QxrdCenterFinderDialog(QWidget *parent)
  : QGroupBox(parent)
{
  setupUi(this);
}

void QxrdCenterFinderDialog::onCenterChanged(double cx, double cy)
{
  m_CenterX -> setValue(cx);
  m_CenterY -> setValue(cy);

  printf("QxrdCenterFinderDialog::onCenterChanged(%g,%g)\n", cx, cy);
}
