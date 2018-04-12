#include "qcepcalibrantdspacings.h"

void QcepCalibrantDSpacings::initialize(QcepObjectWPtr parent)
{
}

void QcepCalibrantDSpacings::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginWriteArray(name, count());

  for (int i=0; i<count(); i++) {
    settings->setArrayIndex(i);
    const QcepCalibrantDSpacing &pt = at(i);
    settings->setValue("index", pt.index());
    settings->setValue("h", pt.h());
    settings->setValue("k", pt.k());
    settings->setValue("l", pt.l());
    settings->setValue("d", pt.d());
    settings->setValue("tth", pt.tth());
  }

  settings->endArray();
}

void QcepCalibrantDSpacings::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QcepCalibrantDSpacings vec = val.value<QcepCalibrantDSpacings>();

  vec.setSettingsValue(settings, name);
}

QString QcepCalibrantDSpacings::toString() const
{
  QString res="[";

  for (int i=0; i<count(); i++) {
    if (i != 0) {
      res += ", \n";
    }

    res += at(i).toString();
  }

  res += "]";

  return res;
}

void QcepCalibrantDSpacings::registerMetaTypes()
{
  qRegisterMetaType< QcepCalibrantDSpacings >("QcepCalibrantDSpacingVector");

  qRegisterMetaTypeStreamOperators< QcepCalibrantDSpacings >("QcepCalibrantDSpacingVector");

  QcepProperty::registerCustomSaver("QcepCalibrantDSpacingVector", QcepCalibrantDSpacings::customSaver);
}

void QcepCalibrantDSpacings::insertUnique(int index, int h, int k, int l, double d, double tth)
{
  int n = count();

  for (int i=0; i<n; i++) {
    QcepCalibrantDSpacing s = value(i);

    if (d == s.d()) {
      s.n() = s.n()+1;

      replace(i, s);

      return;
    }
  }

  append(QcepCalibrantDSpacing(index,h,k,l,1,d,tth));
}

void QcepCalibrantDSpacings::merge(const QcepCalibrantDSpacings &vec)
{
  QcepCalibrantDSpacings a = *this;
  QcepCalibrantDSpacings b = vec;

  clear();

  while (a.count() > 0 || b.count() > 0) {
    if (a.count() > 0 && b.count() > 0) {
      if (a.first().d() > b.first().d()) {
        append(a.takeFirst());
      } else {
        append(b.takeFirst());
      }
    } else if (a.count() > 0) {
      append(a.takeFirst());
    } else if (b.count() > 0) {
      append(b.takeFirst());
    }
  }
}

double QcepCalibrantDSpacings::calibrantDSpacing(int n)
{
  double res = qQNaN();

  if (n >= 0 && n < count()) {
    res = value(n).d();
  }

  return res;
}

double QcepCalibrantDSpacings::calibrantTTH(int n)
{
  double res = qQNaN();

  if (n >= 0 && n < count()) {
    res = value(n).tth();
  }

  return res;
}
