#include "qxrdmaskstack.h"
#include "qcepmaskdata.h"

QxrdMaskStack::QxrdMaskStack(QString name) :
  QcepObject(name),
  QStack<QcepMaskDataPtr>()
{
#ifndef QT_NO_DEBUG
  printf("Constructing mask stack\n");
#endif
}

QxrdMaskStack::~QxrdMaskStack()
{
#ifndef QT_NO_DEBUG
  printf("Deleting mask stack\n");
#endif
}

void QxrdMaskStack::readSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  int n = settings -> beginReadArray("masks");

  for (int i=0; i<n; i++) {
    settings -> setArrayIndex(i);

    QcepObjectPtr obj = QcepObject::readObject(settings);
    QcepMaskDataPtr m =
        qSharedPointerDynamicCast<QcepMaskData>(obj);

    if (m) {
      push_back(m);
    }
  }

  settings -> endArray();
}

void QxrdMaskStack::writeSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  settings -> beginWriteArray("masks");

  for (int i=0; i<count(); i++) {
    settings -> setArrayIndex(i);

    QcepMaskDataPtr m = value(i);

    if (m) {
      m -> writeSettings(settings);
    }
  }

  settings -> endArray();
}

void QxrdMaskStack::changed()
{
  emit maskChanged();
}

QString QxrdMaskStack::stackLevelName(int n)
{
  return tr("XYZTabcdefghijklmnopqrstuvwxyz").at(n);
}

QcepMaskDataPtr QxrdMaskStack::mask()
{
  return value(0);
}

QcepMaskDataPtr QxrdMaskStack::mask(int n)
{
  return value(n);
}

QcepMaskDataPtr QxrdMaskStack::mask(QModelIndex m)
{
  if (m.column() == 0 && m.row() >= 0 && m.row() < count()) {
    return value(m.row());
  } else {
    return QcepMaskDataPtr();
  }
}

int QxrdMaskStack::maskCount()
{
  return count();
}

int QxrdMaskStack::maskStackPosition(int pos)
{
  int len = count();

  if (pos >= 0 && pos < len) {
    return pos;
  } else if (pos < 0 && pos >= -len) {
    return len-pos;
  } else {
    return -1;
  }
}

//void QxrdMaskStack::newMask()
//{

//}

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

void QxrdMaskStack::popMaskStack(int amount)
{
  if (amount == 0) {
    return;
  } else if (amount > 1) {
    for (int i=0; i<amount; i++) {
      popMaskStack(1);
    }
  } else if (amount < -1) {
    for (int i=0; i<(-amount); i++) {
      popMaskStack(-1);
    }
  } else if (amount == 1) {
    if (maskCount() >= 1) {
//      m_Mask = m_Masks.first();
      pop_front();
    }
  } else if (amount == -1) {
    if (maskCount() >= 1) {
//      m_Mask = m_Masks.last();
      pop_back();
    }
  }

  emit maskChanged();
}

void QxrdMaskStack::clearMaskStack()
{
  clear();

  emit maskChanged();
}

void QxrdMaskStack::clearMaskStackTop()
{
  popMaskStack();
}

void QxrdMaskStack::rollMaskStack(int amount)
{
  if (amount == 0) {
    return;
  } else if (amount > 1) {
    for (int i=0; i<amount; i++) {
      rollMaskStack(1);
    }
  } else if (amount < -1) {
    for (int i=0; i<(-amount); i++) {
      rollMaskStack(-1);
    }
  } else if (amount == 1) {
    QcepMaskDataPtr m = first();
    push_back(m);
    pop_front();
  } else if (amount == -1) {
    QcepMaskDataPtr m = last();
    push_front(m);
    pop_back();
  }

  emit maskChanged();
}

void QxrdMaskStack::exchangeMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  QcepMaskDataPtr m0 = mask(p0);
  QcepMaskDataPtr m1 = mask(p1);

  if (m0 && m1) {
    replace(p0,m1);
    replace(p1,m0);

    emit maskChanged();
  }
}

void QxrdMaskStack::andMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  QcepMaskDataPtr m0 = mask(p0);
  QcepMaskDataPtr m1 = mask(p1);

  if (m0 && m1) {
    m0 -> andMask(m1);

    emit maskChanged();
  }
}

void QxrdMaskStack::orMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  QcepMaskDataPtr m0 = mask(p0);
  QcepMaskDataPtr m1 = mask(p1);

  if (m0 && m1) {
    m0 -> orMask(m1);

    emit maskChanged();
  }
}

void QxrdMaskStack::xorMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  QcepMaskDataPtr m0 = mask(p0);
  QcepMaskDataPtr m1 = mask(p1);

  if (m0 && m1) {
    m0 -> xorMask(m1);

    emit maskChanged();
  }
}

void QxrdMaskStack::andNotMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  QcepMaskDataPtr m0 = mask(p0);
  QcepMaskDataPtr m1 = mask(p1);

  if (m0 && m1) {
    m0 -> andNotMask(m1);

    emit maskChanged();
  }
}

void QxrdMaskStack::orNotMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  QcepMaskDataPtr m0 = mask(p0);
  QcepMaskDataPtr m1 = mask(p1);

  if (m0 && m1) {
    m0 -> orNotMask(m1);

    emit maskChanged();
  }
}

void QxrdMaskStack::xorNotMaskStack(int pos)
{
  int p0 = maskStackPosition(0);
  int p1 = maskStackPosition(pos);

  QcepMaskDataPtr m0 = mask(p0);
  QcepMaskDataPtr m1 = mask(p1);

  if (m0 && m1) {
    m0 -> xorNotMask(m1);

    emit maskChanged();
  }
}

void QxrdMaskStack::invertMaskStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> invertMask();

    emit maskChanged();
  }
}

void QxrdMaskStack::growMaskStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> growMask();

    emit maskChanged();
  }
}

void QxrdMaskStack::shrinkMaskStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> shrinkMask();

    emit maskChanged();
  }
}

void QxrdMaskStack::hideMaskAllStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> hideMaskAll();

    emit maskChanged();
  }
}

void QxrdMaskStack::showMaskAllStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> showMaskAll();

    emit maskChanged();
  }
}

void QxrdMaskStack::hideMaskRangeStack(int pos,
                                       QcepImageDataBasePtr img,
                                       double minVal,
                                       double maxVal)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m && img) {
    m -> hideMaskRange(img, minVal, maxVal);

    emit maskChanged();
  }
}

void QxrdMaskStack::showMaskRangeStack(int pos,
                                       QcepImageDataBasePtr img,
                                       double minVal,
                                       double maxVal)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m && img) {
    m -> showMaskRange(img, minVal, maxVal);

    emit maskChanged();
  }
}

