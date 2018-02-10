#include "qxrdcalibrantdspacings.h"

void QxrdCalibrantDSpacings::initialize(QcepObjectWPtr parent)
{
}

void QxrdCalibrantDSpacings::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginWriteArray(name, count());

  for (int i=0; i<count(); i++) {
    settings->setArrayIndex(i);
    const QxrdCalibrantDSpacing &pt = at(i);
    settings->setValue("index", pt.index());
    settings->setValue("h", pt.h());
    settings->setValue("k", pt.k());
    settings->setValue("l", pt.l());
    settings->setValue("d", pt.d());
    settings->setValue("tth", pt.tth());
  }

  settings->endArray();
}

void QxrdCalibrantDSpacings::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QxrdCalibrantDSpacings vec = val.value<QxrdCalibrantDSpacings>();

  vec.setSettingsValue(settings, name);
}

QString QxrdCalibrantDSpacings::toString() const
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

void QxrdCalibrantDSpacings::registerMetaTypes()
{
  qRegisterMetaType< QxrdCalibrantDSpacings >("QxrdCalibrantDSpacingVector");

  qRegisterMetaTypeStreamOperators< QxrdCalibrantDSpacings >("QxrdCalibrantDSpacingVector");

  QcepProperty::registerCustomSaver("QxrdCalibrantDSpacingVector", QxrdCalibrantDSpacings::customSaver);
}

void QxrdCalibrantDSpacings::insertUnique(int index, int h, int k, int l, double d, double tth)
{
  int n = count();

  for (int i=0; i<n; i++) {
    QxrdCalibrantDSpacing s = value(i);

    if (d == s.d()) {
      s.n() = s.n()+1;

      replace(i, s);

      return;
    }
  }

  append(QxrdCalibrantDSpacing(index,h,k,l,1,d,tth));
}

void QxrdCalibrantDSpacings::merge(const QxrdCalibrantDSpacings &vec)
{
  QxrdCalibrantDSpacings a = *this;
  QxrdCalibrantDSpacings b = vec;

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

double QxrdCalibrantDSpacings::calibrantDSpacing(int n)
{
  double res = qQNaN();

  if (n >= 0 && n < count()) {
    res = value(n).d();
  }

  return res;
}

double QxrdCalibrantDSpacings::calibrantTTH(int n)
{
  double res = qQNaN();

  if (n >= 0 && n < count()) {
    res = value(n).tth();
  }

  return res;
}
