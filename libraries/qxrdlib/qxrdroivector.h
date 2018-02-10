#ifndef QXRDROIVECTOR_H
#define QXRDROIVECTOR_H

#include "qxrdlib_global.h"
#include "qcepobject.h"
#include <QVector>
#include "qxrdroi-ptr.h"

//TODO: implement as ROI data store
class QXRD_EXPORT QxrdROIVector : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdROIVector(QString name);

  void initialize(QcepObjectWPtr parent);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  int count() const;

  void append(QxrdROIPtr roi);
  void remove(int i);

  void exchange(int i1, int i2);

  QxrdROIPtr roi(int i);
  void setRoi(int i, const QxrdROIPtr &r);

signals:
  void roisChanged();
  void roiChanged(int i);

private:
  QVector<QxrdROIPtr> m_ROICoordinates;
};

Q_DECLARE_METATYPE(QxrdROIVector*)

#endif // QXRDROIVECTOR_H
