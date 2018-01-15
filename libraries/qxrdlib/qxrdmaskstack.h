#ifndef QXRDMASKSTACK_H
#define QXRDMASKSTACK_H

#include "qxrdlib_global.h"
#include <QStack>
#include <QModelIndexList>
#include "qcepobject.h"
#include "qcepmaskdata-ptr.h"
#include "qcepimagedata-ptr.h"

//TODO: fix shared pointer leaks
class QXRD_EXPORT QxrdMaskStack : public QcepObject, public QStack<QcepMaskDataPtr>
{
  Q_OBJECT

public:
  Q_INVOKABLE QxrdMaskStack(QString name);
  virtual ~QxrdMaskStack();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void changed();
  static QString stackLevelName(int n);

  QcepMaskDataPtr mask();
  QcepMaskDataPtr mask(int n);
  QcepMaskDataPtr mask(QModelIndex m);
  int             maskCount();

//  void newMask();
  void enableMasks(QModelIndexList sel);
  void disableMasks(QModelIndexList sel);
  void deleteMasks(QModelIndexList sel);
  void andMasks(QModelIndexList sel);
  void orMasks(QModelIndexList sel);

  void clearMaskStack();
  void clearMaskStackTop();
  void rollMaskStack(int amount);
  void exchangeMaskStack(int pos);
  void andMaskStack(int pos);
  void orMaskStack(int pos);
  void xorMaskStack(int pos);
  void andNotMaskStack(int pos);
  void orNotMaskStack(int pos);
  void xorNotMaskStack(int pos);
  void invertMaskStack(int pos);
  void growMaskStack(int pos);
  void shrinkMaskStack(int pos);
  void hideMaskAllStack(int pos);
  void showMaskAllStack(int pos);
  void hideMaskRangeStack(int pos, QcepImageDataBasePtr img, double minVal, double maxVal);
  void showMaskRangeStack(int pos, QcepImageDataBasePtr img, double minVal, double maxVal);

  typedef enum {
    ExcludeInside,
    ExcludeOutside,
    IncludeInside,
    IncludeOutside
  } QxrdThresholdMode;

  void thresholdMasks(QModelIndexList sel, QxrdThresholdMode mode, double low, double high, QcepDoubleImageData *data);

private:
  int maskStackPosition(int pos);
  void popMaskStack(int amount=1);

signals:
  void maskChanged();
};

Q_DECLARE_METATYPE(QxrdMaskStack*)

#endif // QXRDMASKSTACK_H
