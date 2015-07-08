#ifndef QXRDIMAGEDATAGRAPHCONTROLLER_H
#define QXRDIMAGEDATAGRAPHCONTROLLER_H

#include "qxrddataobjectgraphcontroller.h"

class QxrdImageDataGraphController : public QxrdDataObjectGraphController
{
  Q_OBJECT

public:
  QxrdImageDataGraphController(QxrdDataObjectGraphWindow *window, int mode, QcepDataObjectPtr object);

  virtual ~QxrdImageDataGraphController();
  virtual void activate();

public slots:
  virtual void updateDisplay();
};

#endif // QXRDIMAGEDATAGRAPHCONTROLLER_H
