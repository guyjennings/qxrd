#ifndef QCEPROIMODEL_H
#define QCEPROIMODEL_H

#include "qceplib_global.h"
#include "qcepobject.h"
#include <QAbstractListModel>
#include "qceproi-ptr.h"
#include "qceproivector-ptr.h"
#include <QScriptEngine>
#include "qceproimodel-ptr.h"
#include "qcepproperty.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QCEP_EXPORT QcepROIModel : public QAbstractListModel
{
  Q_OBJECT

public:
  QcepROIModel(QcepROIVectorWPtr rois);
  virtual ~QcepROIModel();

  void initialize(QcepObjectWPtr parent);

//  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepROIModelPtr &coords);
//  static void fromScriptValue(const QScriptValue &obj, QcepROIModelPtr &coords);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  void append(QcepROIPtr coords);
  void removeROI(int row);

  void moveROIDown(int row);
  void moveROIUp(int row);

  void editROI(int row);

  void moveROICenter(int i, double x, double y);
  void moveROIRelative(int i, double dx, double dy);
  void setRotation(int i, double r);
  void scaleROI(int i, int innerOuter, double kx, double ky);

  void deleteROIPoint(int i, int innerOuter, int n);
  void changeROIPoint(int i, int innerOuter, int n, QPointF pt);
  void insertROIPoint(int i, int innerOuter, int n, QPointF pt);

  QcepROIWPtr roi(int row) const;
  void setRoi(int row, QcepROIPtr c);

  int roiCount() const;

  void recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
  void visualizeBackground(int n, QcepImageDataBasePtr img, QcepMaskDataPtr mask);
  void visualizePeak(int n, QcepImageDataBasePtr img, QcepMaskDataPtr mask);

  enum {
    NumCol,
    SumCol,
    AverageCol,
    MinimumCol,
    MaximumCol,
    NPixelsCol,
    BackgroundCol,
    XGradientCol,
    YGradientCol,
    OuterTypeCol,
    InnerTypeCol,
    CenterXCol,
    CenterYCol,
    RotationCol,
//    WidthCol,
//    HeightCol,
//    Width2Col,
//    Height2Col,
    ColCount
  };

signals:

public slots:
  void onROIChanged();
  void onROIsChanged();

private:
  QcepROIPtr newROI(int roiOuterType, int roiInnerType);

private:
  QcepROIVectorWPtr   m_ROIs;
};

#endif // QCEPROIMODEL_H
