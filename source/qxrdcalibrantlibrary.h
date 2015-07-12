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
  QxrdCalibrantLibrary(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp);
  ~QxrdCalibrantLibrary();
  void initialize(QxrdCalibrantLibraryWPtr lib);
  QxrdCalibrantLibraryWPtr calibrantLibraryPtr() const;

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
  QcepSettingsSaverWPtr     m_Saver;
  QxrdExperimentWPtr        m_Experiment;
  QxrdCalibrantLibraryWPtr  m_CalibrantLibrary;
  QVector<QxrdCalibrantPtr> m_Calibrants;
};

#endif // QXRDCALIBRANTLIBRARY_H
