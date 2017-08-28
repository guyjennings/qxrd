#ifndef QCEPIMAGESLICEGRAPHCONTROLLER_H
#define QCEPIMAGESLICEGRAPHCONTROLLER_H

#include "qceplib_global.h"
#include "qcepdataobjectgraphcontroller.h"

class QCEP_EXPORT QcepImageSliceGraphController : public QcepDataObjectGraphController
{
  Q_OBJECT

public:
  QcepImageSliceGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);

  virtual ~QcepImageSliceGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QCEPIMAGESLICEGRAPHCONTROLLER_H
