#ifndef QCEPCALIBRANTDSPACINGS_H
#define QCEPCALIBRANTDSPACINGS_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include <QVector>
#include "qcepcalibrantdspacing.h"
#include <QSettings>
#include "qcepproperty.h"

class QCEP_EXPORT QcepCalibrantDSpacings : public QVector<QcepCalibrantDSpacing>
{
public:
  void initialize(QcepObjectWPtr parent);

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  QString toString() const;

  static void registerMetaTypes();

  void insertUnique(int index, int h, int k, int l, double d, double tth);

  void merge(const QcepCalibrantDSpacings& vec);

  double calibrantDSpacing(int n);
  double calibrantTTH(int n);
};

Q_DECLARE_METATYPE(QcepCalibrantDSpacings)

#endif // QCEPCALIBRANTDSPACINGS_H
