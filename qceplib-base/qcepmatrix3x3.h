#ifndef QCEPMATRIX3X3_H
#define QCEPMATRIX3X3_H

#include <QMatrix3x3>
#include <QMetaType>
#include "qceppropertyvalue.h"

class QcepMatrix3x3 : public QcepPropertyValue, public QMatrix3x3
{
public:
  void setSettingsValue(QSettings *settings, QString name);

  static void customSaver(const QVariant &val, QSettings *settings, QString name);
};

Q_DECLARE_METATYPE(QcepMatrix3x3)

#endif // QCEPMATRIX3X3_H
