#ifndef QXRDCALIBRANTDSPACINGS_H
#define QXRDCALIBRANTDSPACINGS_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include <QVector>
#include "qxrdcalibrantdspacing.h"
#include <QSettings>
#include "qcepproperty.h"

class QXRD_EXPORT QxrdCalibrantDSpacings : public QVector<QxrdCalibrantDSpacing>
{
public:
  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  QString toString() const;

  static void registerMetaTypes();
//  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdCalibrantDSpacingVector &vec);
//  static void fromScriptValue(const QScriptValue &obj, QxrdCalibrantDSpacingVector &vec);

  void insertUnique(int index, int h, int k, int l, double d, double tth);

  void merge(const QxrdCalibrantDSpacings& vec);

  double calibrantDSpacing(int n);
  double calibrantTTH(int n);
};

Q_DECLARE_METATYPE(QxrdCalibrantDSpacings)

#endif // QXRDCALIBRANTDSPACINGS_H
