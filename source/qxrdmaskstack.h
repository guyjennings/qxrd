#ifndef QXRDMASKSTACK_H
#define QXRDMASKSTACK_H

#include <QStack>
#include <QModelIndexList>

#include "qxrdmaskdata.h"
#include "qxrdimagedata.h"

class QxrdMaskStack : public QObject, public QStack<QxrdMaskDataPtr>
{
  Q_OBJECT;

public:
  QxrdMaskStack();

  void changed();
  static QString stackLevelName(int n);

  QxrdMaskDataPtr mask(QModelIndex m);

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

  void thresholdMasks(QModelIndexList sel, QxrdMaskStack::QxrdThresholdMode mode, double low, double high, QxrdDoubleImageData *data);

signals:
  void maskChanged();
};

typedef QxrdMaskStack *QxrdMaskStackPtr;

#endif // QXRDMASKSTACK_H
