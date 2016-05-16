#ifndef QXRDCALIBRANTLIBRARY_H
#define QXRDCALIBRANTLIBRARY_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qxrdcalibrant-ptr.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdcalibrantlibrary-ptr.h"

class QxrdCalibrantLibrary : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdCalibrantLibrary(QString name);
  QxrdCalibrantLibrary();
  virtual ~QxrdCalibrantLibrary();

  virtual void addChildPtr(QcepObjectPtr child);

public slots:
  int count();
  QString calibrantName(int n);
  QxrdCalibrantWPtr calibrant(int n);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

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
