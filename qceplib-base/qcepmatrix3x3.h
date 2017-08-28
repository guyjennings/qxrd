#ifndef QCEPMATRIX3X3_H
#define QCEPMATRIX3X3_H

#include "qceplib_global.h"
#include <QMatrix3x3>
#include <QMetaType>
#include "qceppropertyvalue.h"

class QCEP_EXPORT QcepMatrix3x3 : public QcepPropertyValue, public QMatrix3x3
{
public:
  void setSettingsValue(QSettings *settings, QString name);

  static void customSaver(const QVariant &val, QSettings *settings, QString name);
};

Q_DECLARE_METATYPE(QcepMatrix3x3)

#endif // QCEPMATRIX3X3_H
