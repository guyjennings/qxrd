#include "qcepmaskstack.h"
#include "qcepmaskdata.h"

QcepMaskStack::QcepMaskStack(QString name) :
  QcepObject(name),
  QStack<QcepMaskDataPtr>()
{
#ifndef QT_NO_DEBUG
  printf("Constructing mask stack\n");
#endif
}

QcepMaskStack::~QcepMaskStack()
{
#ifndef QT_NO_DEBUG
  printf("Deleting mask stack\n");
#endif
}

void QcepMaskStack::readSettings(QSettings *settings)
{
  QcepObject::readSettings(settings);

  int n = settings -> beginReadArray("masks");

  for (int i=0; i<n; i++) {
    splashMessage(tr("Reading mask stack entry %1 of %2").arg(i).arg(n));

    settings -> setArrayIndex(i);

    QcepObjectPtr obj = QcepObject::readObject(sharedFromThis(), settings);

    if (obj) {
      QcepMaskDataPtr m =
          qSharedPointerDynamicCast<QcepMaskData>(obj);

      if (m) {
        push_back(m);
      }
    }
  }

  settings -> endArray();
}

void QcepMaskStack::writeSettings(QSettings *settings)
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

void QcepMaskStack::changed()
{
  emit maskChanged();
}

QString QcepMaskStack::stackLevelName(int n)
{
  return tr("XYZTabcdefghijklmnopqrstuvwxyz").at(n);
}

void QcepMaskStack::push(QcepMaskDataPtr m)
{
  inherited::push(m);

  emit maskChanged();
}

QcepMaskDataPtr QcepMaskStack::pop()
{
  QcepMaskDataPtr res;

  if (maskCount() > 0) {
    res = inherited::takeFirst();
  }

  emit maskChanged();

  return res;
}

QcepMaskDataPtr QcepMaskStack::first()
{
  QcepMaskDataPtr res;

  if (maskCount() > 0) {
    res = inherited::first();
  }

  return res;
}

bool QcepMaskStack::isEmpty()
{
  return maskCount() == 0;
}

QcepMaskDataPtr QcepMaskStack::mask()
{
  return value(0);
}

QcepMaskDataPtr QcepMaskStack::mask(int n)
{
  return value(n);
}

QcepMaskDataPtr QcepMaskStack::mask(QModelIndex m)
{
  if (m.column() == 0 && m.row() >= 0 && m.row() < count()) {
    return value(m.row());
  } else {
    return QcepMaskDataPtr();
  }
}

int QcepMaskStack::maskCount()
{
  return count();
}

int QcepMaskStack::maskStackPosition(int pos)
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

//void QcepMaskStack::newMask()
//{

//}

void QcepMaskStack::enableMasks(QModelIndexList sel)
{
  foreach (QModelIndex m, sel) {
    QcepMaskDataPtr data = mask(m);

    if (data) {
      data->set_Used(true);
    }
  }

  emit maskChanged();
}

void QcepMaskStack::disableMasks(QModelIndexList sel)
{
  foreach (QModelIndex m, sel) {
    QcepMaskDataPtr data = mask(m);

    if (data) {
      data->set_Used(false);
    }
  }

  emit maskChanged();
}

void QcepMaskStack::deleteMasks(QModelIndexList /*sel*/)
{

}

void QcepMaskStack::andMasks(QModelIndexList /*sel*/)
{

}

void QcepMaskStack::orMasks(QModelIndexList /*sel*/)
{

}

void QcepMaskStack::popMaskStack(int amount)
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

void QcepMaskStack::clearMaskStack()
{
  clear();

  emit maskChanged();
}

void QcepMaskStack::clearMaskStackTop()
{
  popMaskStack();
}

void QcepMaskStack::rollMaskStack(int amount)
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

void QcepMaskStack::exchangeMaskStack(int pos)
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

void QcepMaskStack::andMaskStack(int pos)
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

void QcepMaskStack::orMaskStack(int pos)
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

void QcepMaskStack::xorMaskStack(int pos)
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

void QcepMaskStack::andNotMaskStack(int pos)
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

void QcepMaskStack::orNotMaskStack(int pos)
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

void QcepMaskStack::xorNotMaskStack(int pos)
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

void QcepMaskStack::invertMaskStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> invertMask();

    emit maskChanged();
  }
}

void QcepMaskStack::growMaskStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> growMask();

    emit maskChanged();
  }
}

void QcepMaskStack::shrinkMaskStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> shrinkMask();

    emit maskChanged();
  }
}

void QcepMaskStack::hideMaskAllStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> hideMaskAll();

    emit maskChanged();
  }
}

void QcepMaskStack::showMaskAllStack(int pos)
{
  int p = maskStackPosition(pos);

  QcepMaskDataPtr m = mask(p);

  if (m) {
    m -> showMaskAll();

    emit maskChanged();
  }
}

void QcepMaskStack::hideMaskRangeStack(int pos,
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

void QcepMaskStack::showMaskRangeStack(int pos,
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

void QcepMaskStack::maskRectangle(const QRectF &r, bool set)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m->maskRectangle(r, set);

    emit maskChanged();
  }
}

void QcepMaskStack::maskCircle(const QRectF &r, bool set)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m->maskCircle(r, set);

    emit maskChanged();
  }
}

void QcepMaskStack::maskPolygon(const QVector<QPointF> &p, bool set)
{
  createMaskIfNeeded();

  QcepMaskDataPtr m(mask());

  if (m) {
    m->maskPolygon(p, set);

    emit maskChanged();
  }
}

//TODO: implement (need width and height)
void QcepMaskStack::createMaskIfNeeded()
{
  if (isEmpty()) {

  }
}
