#ifndef QXRDMASKSTACK_H
#define QXRDMASKSTACK_H

#include <QStack>
#include "qxrdmaskdata.h"

class QxrdMaskStack : public QObject, public QStack<QxrdMaskDataPtr>
{
  Q_OBJECT;

public:
  QxrdMaskStack();

  void changed();
  QString stackLevelName(int n);
signals:
  void maskChanged();
};

typedef QxrdMaskStack *QxrdMaskStackPtr;

#endif // QXRDMASKSTACK_H
