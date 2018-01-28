#ifndef QXRDCALIBRANTLIBRARY_H
#define QXRDCALIBRANTLIBRARY_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qxrdcalibrant-ptr.h"
#include "qcepproperty.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdcalibrantlibrary-ptr.h"

class QXRD_EXPORT QxrdCalibrantLibrary : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCalibrantLibrary(QString name);
  virtual ~QxrdCalibrantLibrary();
  static QxrdCalibrantLibraryPtr newCalibrantLibrary();

public slots:
  int count();
  QString calibrantName(int n);
  QxrdCalibrantWPtr calibrant(int n);

public:
  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void removeCalibrant();
  void appendCalibrant(QxrdCalibrantPtr cal = QxrdCalibrantPtr());
  int numberStandardCalibrants();
  QxrdCalibrantPtr standardCalibrant(int n);

public:

private:
  mutable QMutex            m_Mutex;
  QxrdExperimentWPtr        m_Experiment;
  QVector<QxrdCalibrantPtr> m_Calibrants;
};

Q_DECLARE_METATYPE(QxrdCalibrantLibrary*)

#endif // QXRDCALIBRANTLIBRARY_H
