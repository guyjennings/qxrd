#ifndef QXRDRINGSETFITPARAMETERS_H
#define QXRDRINGSETFITPARAMETERS_H

#include "qxrdfitparameter.h"
#include "qxrdringfitparameters.h"
#include "qxrdsettings.h"

class QxrdRingSetFitParameters : public QObject
{
  Q_OBJECT
public:
  explicit QxrdRingSetFitParameters(QObject *parent = 0);

public:
  Q_PROPERTY(double centerX READ get_CenterX WRITE set_CenterX)
  QXRD_FIT_PARAMETER(CenterX)

  Q_PROPERTY(double centerY READ get_CenterY WRITE set_CenterY)
  QXRD_FIT_PARAMETER(CenterY)

  Q_PROPERTY(double distance READ get_Distance WRITE set_Distance)
  QXRD_FIT_PARAMETER(Distance)

  Q_PROPERTY(double energy READ get_Energy WRITE set_Energy)
  QXRD_FIT_PARAMETER(Energy)

  Q_PROPERTY(double tilt READ get_Tilt WRITE set_Tilt)
  QXRD_FIT_PARAMETER(Tilt)

  Q_PROPERTY(double rotation READ get_Rotation WRITE set_Rotation)
  QXRD_FIT_PARAMETER(Rotation)

public slots:
  QList<QxrdRingFitParameters*> rings() const;
  QxrdRingFitParameters* ring(int n) const;
  int count() const;
  void clear();
  void append(double twoTheta=0);
  void remove(int n);
  void show();

  double ellipseR(double chi, double eccen, double a);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

private:
  mutable QMutex         m_Mutex;

  QList<QxrdRingFitParametersPtr> m_Rings;
};

typedef QxrdRingSetFitParameters *QxrdRingSetFitParametersPtr;

Q_DECLARE_METATYPE(QxrdRingSetFitParametersPtr)

#endif // QXRDRINGSETFITPARAMETERS_H
