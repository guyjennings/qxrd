#ifndef QCEPIMAGESLICEGRAPHCONTROLLER_H
#define QCEPIMAGESLICEGRAPHCONTROLLER_H

#include "qcepdataobjectgraphcontroller.h"

class QcepImageSliceGraphController : public QcepDataObjectGraphController
{
  Q_OBJECT

public:
  QcepImageSliceGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);

  virtual ~QcepImageSliceGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QCEPIMAGESLICEGRAPHCONTROLLER_H
