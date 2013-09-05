#include "qcepvector3d.h"

QcepVector3D::QcepVector3D()
{
}

QcepVector3D::QcepVector3D(double xpos, double ypos, double zpos) :
  QcepPropertyValue(),
  QVector3D(xpos, ypos, zpos)
{
}

void QcepVector3D::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);

  settings->setValue("x", x());
  settings->setValue("y", y());
  settings->setValue("z", z());

  settings->endGroup();
}

void QcepVector3D::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QcepVector3D vec = val.value<QcepVector3D>();

  vec.setSettingsValue(settings, name);
}
