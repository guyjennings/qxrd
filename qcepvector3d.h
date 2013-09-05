#ifndef QCEPVECTOR3D_H
#define QCEPVECTOR3D_H

#include <QVector3D>
#include <QMetaType>

#include "qceppropertyvalue.h"

class QcepVector3D : public QcepPropertyValue, public QVector3D
{
public:
  QcepVector3D();
  QcepVector3D(double xpos, double ypos, double zpos);

public:
  void setSettingsValue(QSettings *settings, QString name);

  static void customSaver(const QVariant &val, QSettings *settings, QString name);
};

Q_DECLARE_METATYPE(QcepVector3D)

#endif // QCEPVECTOR3D_H
