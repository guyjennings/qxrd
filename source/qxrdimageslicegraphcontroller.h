#ifndef QXRDIMAGESLICEGRAPHCONTROLLER_H
#define QXRDIMAGESLICEGRAPHCONTROLLER_H

#include "qxrddataobjectgraphcontroller.h"

class QxrdImageSliceGraphController : public QxrdDataObjectGraphController
{
  Q_OBJECT

public:
  QxrdImageSliceGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);

  virtual ~QxrdImageSliceGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QXRDIMAGESLICEGRAPHCONTROLLER_H
