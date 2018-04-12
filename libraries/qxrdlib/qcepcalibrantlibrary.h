#ifndef QCEPCALIBRANTLIBRARY_H
#define QCEPCALIBRANTLIBRARY_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepcalibrant-ptr.h"
#include "qcepproperty.h"
#include "qcepcalibrantlibrary-ptr.h"

class QCEP_EXPORT QcepCalibrantLibrary : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QcepCalibrantLibrary(QString name);
  virtual ~QcepCalibrantLibrary();
  void initialize(QcepObjectWPtr parent);

public slots:
  int count();
  QString calibrantName(int n);
  QcepCalibrantWPtr calibrant(int n);

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void removeCalibrant();
  void appendCalibrant(QcepCalibrantPtr cal = QcepCalibrantPtr());
  int numberStandardCalibrants();
  QcepCalibrantPtr standardCalibrant(QcepObjectWPtr parent, int n);

  enum {
    SiliconCalibrant,
    AluminaCalibrant,
    NACCalibrant,
    LaB6Calibrant,
    ZnOCalibrant,
    CeO2Calibrant,
    Cr2O3Calibrant,
    GeCalibrant,
    StandardCalibrantCount
  };
public:

private:
  QVector<QcepCalibrantPtr> m_Calibrants;
};

Q_DECLARE_METATYPE(QcepCalibrantLibrary*)

#endif // QCEPCALIBRANTLIBRARY_H
