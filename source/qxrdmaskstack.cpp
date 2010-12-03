#include "qxrdmaskstack.h"

QxrdMaskStack::QxrdMaskStack() : QStack<QxrdMaskDataPtr>()
{
}

void QxrdMaskStack::changed()
{
  emit maskChanged();
}
