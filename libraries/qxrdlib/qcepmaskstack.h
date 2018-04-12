#ifndef QCEPMASKSTACK_H
#define QCEPMASKSTACK_H

#include "qceplib_global.h"
#include <QStack>
#include <QModelIndexList>
#include "qcepobject.h"
#include "qcepmaskdata-ptr.h"
#include "qcepimagedata-ptr.h"

class QCEP_EXPORT QcepMaskStack : public QcepObject, private QStack<QcepMaskDataPtr>
{
  Q_OBJECT

public:
  typedef QStack<QcepMaskDataPtr> inherited;

  Q_INVOKABLE QcepMaskStack(QString name);
  virtual ~QcepMaskStack();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  void changed();
  static QString stackLevelName(int n);

  void push(QcepMaskDataPtr m);
  QcepMaskDataPtr pop();
  QcepMaskDataPtr first();

  bool isEmpty();

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

  void createMaskIfNeeded();

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

  void maskRectangle(const QRectF& r, bool set);
  void maskCircle(const QRectF& r, bool set);
  void maskPolygon(const QVector<QPointF> &p, bool set);

  typedef enum {
    ExcludeInside,
    ExcludeOutside,
    IncludeInside,
    IncludeOutside
  } QcepThresholdMode;

  void thresholdMasks(QModelIndexList sel, QcepThresholdMode mode, double low, double high, QcepDoubleImageData *data);

private:
  int maskStackPosition(int pos);
  void popMaskStack(int amount=1);

signals:
  void maskChanged();
};

Q_DECLARE_METATYPE(QcepMaskStack*)

#endif // QCEPMASKSTACK_H
