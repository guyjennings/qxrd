#ifndef QCEPIMAGEDATAGRAPHCONTROLLER_H
#define QCEPIMAGEDATAGRAPHCONTROLLER_H

#include "qceplib_global.h"
#include "qcepdataobjectgraphcontroller.h"

class QCEP_EXPORT QcepImageDataGraphController : public QcepDataObjectGraphController
{
  Q_OBJECT

public:
  QcepImageDataGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object);

  virtual ~QcepImageDataGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QCEPIMAGEDATAGRAPHCONTROLLER_H
