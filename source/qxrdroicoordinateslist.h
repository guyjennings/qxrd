#ifndef QXRDROICOORDINATESLIST_H
#define QXRDROICOORDINATESLIST_H

#include "qcepobject.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdroicoordinates-ptr.h"
#include <QVector>
#include "qxrdroicoordinateslist-ptr.h"
#include <QScriptEngine>
#include "qcepproperty.h"

class QxrdROICoordinatesList : public QcepObject
{
  Q_OBJECT

public:
  QxrdROICoordinatesList(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp);
  virtual ~QxrdROICoordinatesList();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesListPtr &coords);
  static void fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesListPtr &coords);

signals:

public slots:
  void clear();
  void appendROI(QxrdROICoordinatesPtr coords);
  void appendROI(int roiType, double left=0, double top=0, double right=0, double bottom=0);
  void removeROI(int i);

  QxrdROICoordinatesPtr roi(int i);
  void setRoi(int i, QxrdROICoordinatesPtr c);

private:
  QxrdROICoordinatesPtr newROI(int roiType);

private:
  QcepSettingsSaverWPtr          m_Saver;
  QMutex                         m_Mutex;
  QxrdExperimentWPtr             m_Experiment;
  QVector<QxrdROICoordinatesPtr> m_ROICoordinates;

public:
  Q_PROPERTY(int roiCount READ get_RoiCount WRITE set_RoiCount)
  QCEP_INTEGER_PROPERTY(RoiCount)
};

#endif // QXRDROICOORDINATESLIST_H
