#include "qxrdmaskstack.h"
#include "qcepmaskdata.h"

QxrdMaskStack::QxrdMaskStack() : QStack<QcepMaskDataPtr>()
{
}

void QxrdMaskStack::changed()
{
  emit maskChanged();
}

QString QxrdMaskStack::stackLevelName(int n)
{
  return tr("XYZTabcdefghijklmnopqrstuvwxyz").at(n);
}

QcepMaskDataPtr QxrdMaskStack::mask(QModelIndex m)
{
  if (m.column() == 0 && m.row() >= 0 && m.row() < count()) {
    return at(m.row());
  } else {
    return QcepMaskDataPtr();
  }
}

void QxrdMaskStack::newMask()
{

}

void QxrdMaskStack::enableMasks(QModelIndexList sel)
{
  foreach (QModelIndex m, sel) {
    QcepMaskDataPtr data = mask(m);

    if (data) {
      data->set_Used(true);
    }
  }

  emit maskChanged();
}

void QxrdMaskStack::disableMasks(QModelIndexList sel)
{
  foreach (QModelIndex m, sel) {
    QcepMaskDataPtr data = mask(m);

    if (data) {
      data->set_Used(false);
    }
  }

  emit maskChanged();
}

void QxrdMaskStack::deleteMasks(QModelIndexList /*sel*/)
{

}

void QxrdMaskStack::andMasks(QModelIndexList /*sel*/)
{

}

void QxrdMaskStack::orMasks(QModelIndexList /*sel*/)
{

}
