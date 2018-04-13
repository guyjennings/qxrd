#ifndef QCEPROIVECTOR_H
#define QCEPROIVECTOR_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include <QVector>
#include "qceproi-ptr.h"

//TODO: implement as ROI data store
class QCEP_EXPORT QcepROIVector : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QcepROIVector(QString name);

  void initialize(QcepObjectWPtr parent);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  int count() const;

  void append(QcepROIPtr roi);
  void remove(int i);

  void exchange(int i1, int i2);

  QcepROIPtr roi(int i);
  void setRoi(int i, const QcepROIPtr &r);

signals:
  void roisChanged();
  void roiChanged(int i);

private:
  QVector<QcepROIPtr> m_ROICoordinates;
};

Q_DECLARE_METATYPE(QcepROIVector*)

#endif // QCEPROIVECTOR_H
