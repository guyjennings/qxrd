#ifndef QXRDROICOORDINATESLISTMODEL_H
#define QXRDROICOORDINATESLISTMODEL_H

#include "qcepobject.h"
#include "qcepsettingssaver-ptr.h"
#include <QAbstractListModel>
#include "qxrdroicoordinates-ptr.h"
#include <QScriptEngine>
#include "qxrdroicoordinateslistmodel-ptr.h"
#include "qcepproperty.h"
#include "qxrdexperiment-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"

class QxrdROICoordinatesListModel : public QAbstractListModel
{
  Q_OBJECT

public:
  QxrdROICoordinatesListModel(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp);
  virtual ~QxrdROICoordinatesListModel();

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesListModelPtr &coords);
  static void fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesListModelPtr &coords);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool setData(const QModelIndex &index, const QVariant &value, int role);

  void append(QxrdROICoordinatesPtr coords);
  void removeROI(int row);

  void moveROIDown(int row);
  void moveROIUp(int row);

  void editROI(int row);

  void moveROICenter(int row, double x, double y);

  QxrdROICoordinatesPtr roi(int row) const;
  void setRoi(int row, QxrdROICoordinatesPtr c);

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
    TypeCol,
    CenterXCol,
    CenterYCol,
    WidthCol,
    HeightCol,
    Width2Col,
    Height2Col,
    ColCount
  };

signals:

public slots:
  void onROIChanged();
  void onROIsChanged();

private:
  QxrdROICoordinatesPtr newROI(int roiType);

private:
  QcepSettingsSaverWPtr          m_Saver;
  QMutex                         m_Mutex;
  QxrdExperimentWPtr             m_Experiment;
  QVector<QxrdROICoordinatesPtr> m_ROICoordinates;
};

#endif // QXRDROICOORDINATESLISTMODEL_H
