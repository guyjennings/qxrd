#ifndef QXRDMASKSTACK_H
#define QXRDMASKSTACK_H

#include <QStack>
#include <QModelIndexList>

#include "qcepmaskdata-ptr.h"
#include "qcepimagedata-ptr.h"

class QxrdMaskStack : public QObject, public QStack<QcepMaskDataPtr>
{
  Q_OBJECT

public:
  QxrdMaskStack();

  void changed();
  static QString stackLevelName(int n);

  QcepMaskDataPtr mask(QModelIndex m);

  void newMask();
  void enableMasks(QModelIndexList sel);
  void disableMasks(QModelIndexList sel);
  void deleteMasks(QModelIndexList sel);
  void andMasks(QModelIndexList sel);
  void orMasks(QModelIndexList sel);

  typedef enum {
    ExcludeInside,
    ExcludeOutside,
    IncludeInside,
    IncludeOutside
  } QxrdThresholdMode;

  void thresholdMasks(QModelIndexList sel, QxrdThresholdMode mode, double low, double high, QcepDoubleImageData *data);

signals:
  void maskChanged();
};

#endif // QXRDMASKSTACK_H
