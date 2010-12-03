#include "qxrdmaskstack.h"

QxrdMaskStack::QxrdMaskStack() : QStack<QxrdMaskDataPtr>()
{
}

void QxrdMaskStack::changed()
{
  emit maskChanged();
}

QString QxrdMaskStack::stackLevelName(int n)
{
  return tr("XYZTabcdefgh").at(n);
}
