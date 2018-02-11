#ifndef QXRDACQUISITIONSCALERMODEL_H
#define QXRDACQUISITIONSCALERMODEL_H

#include "qxrdlib_global.h"
#include <QAbstractListModel>
#include "qxrdacqcommon-ptr.h"

class QXRD_EXPORT QxrdAcquisitionScalerModel : public QAbstractListModel
{
  Q_OBJECT

public:
  QxrdAcquisitionScalerModel(QxrdAcqCommonWPtr acq);

  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;

  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  enum {
    NumCol,
    ValueCol,
    DescriptionCol,
    ColCount
  };
signals:

public slots:
  void forceFullUpdate();

private:
  QxrdAcqCommonWPtr m_Acquisition;
};

#endif // QXRDACQUISITIONSCALERMODEL_H
