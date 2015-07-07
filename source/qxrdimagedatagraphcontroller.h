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
};

#endif // QXRDIMAGEDATAGRAPHCONTROLLER_H
